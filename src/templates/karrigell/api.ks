import json
import sys
import traceback
from Queue import Empty
from multiprocessing import Process,Queue
from time import time

Include('../import.py')
import premia.assets
import premia.enum

def _return(x):
   print json.dumps(x)
   
def _lookupAsset(a):
   return eval('premia.assets.'+a)
   
def _lookupModel(m):
   return eval('premia.models.'+m)
   
def _lookupOption(f, o):
   return eval('premia.options.'+f+'.'+o)
   
def _lookupMethod(m, f, meth):
   m_id = _lookupModel(m).ID()
   return eval('premia.pricings.'+ m_id + '.' + m_id + '_' + f + '.' + meth)

def assets():
   _return(map(lambda a: a.__name__, premia.assets.all()))
   
def models(a):
   _return(map(lambda m: m.__name__, _lookupAsset(a).models()))
   
def families(m):
   _return(_lookupModel(m).families())

def options(m, f):
   m_id = _lookupModel(m).ID()
   opts = eval('pricings.' + m_id + '.' + m_id + '_' + f + '.methods_for_options()').keys()
   _return(map(lambda o: o.__name__, opts))
   
def methods(m, f, o):
   m_id = _lookupModel(m).ID()
   opt = _lookupOption(f, o)
   methods = [x.__name__ for x in eval('pricings.' + m_id + '.' + m_id + '_' + f + '.all()') if opt in x.options()]
   _return(methods)

def _params(obj):
   return obj.meta()
   
def model_params(m):
   _return(_params(_lookupModel(m)()))

def option_params(f, o):
   _return(_params(_lookupOption(f, o)()))

def method_params(m, f, meth):
   _return(_params(_lookupMethod(m, f, meth)()))

def myImport(m):
   exec 'import ' + m
   return eval(m)

def _lookupEnum(e):
   exec 'import premia.enum.'+e
   return eval('premia.enum.'+e+'.'+e)

def enum_params(e):
   obj = _lookupEnum(e).meta()
   _return([(k, v.meta()) for (k,v) in obj]) 

def _parse(kwargs):
   request = kwargs.iterkeys().__iter__().next()
   return json.loads(request)

def id(*args, **kwargs):
   print str(_parse(kwargs)['e'])

def roundIfNeeded(dstType):
   if dstType == type(1):
      return lambda x: int(round(x))
   if dstType == type(1L):
      return lambda x: long(round(x))
   return lambda x: x

def iterate(initial, limit, stepsNo):

   cast = roundIfNeeded(type(initial))

   for i in range(stepsNo):
      x = initial + 1. * i / (stepsNo - 1) * (limit - initial) if stepsNo > 1 else initial
      yield cast(x)     

class Iterable(object):

   def __init__(self, label, setter, startValue, stopValue, interationsNo):
      self.keys = list(iterate(startValue, stopValue, interationsNo))
      self.name = label
      self.setter = setter
      self.stepsNo = interationsNo

_timeout = 30

def wrap_exc(F, q, *args):
   try:
      F(q, *args)
   except Exception, exc:
      res = [("Exception", str(exc)+traceback.format_tb())]
      q.put(res)
      return 

def _compute_scalar(model_obj, option_obj, method_obj):
   res = []
   def F(q, opt, mod):
      begin = time()
      res = method_obj(opt, mod)
      end = time()
      res.append(("Time", end - begin))
      q.put(res)
   try:
      queue = Queue()
      process = Process(target = wrap_exc, args = (F, queue, option_obj,model_obj))
      process.start()
      try:
         res = queue.get(timeout=_timeout)
         if res[0][0] == "Exception":
            res = None
      except Empty, exc:
         process.terminate()
         raise Exception("Method has worked more than " + str(_timeout) + "s. Please try another parameter combination")
      process.join(timeout=_timeout)
   except Exception, exc:
      pass
   return res 

def _compute_iteration_1d(model_obj, option_obj, method_obj, iteration):

   def G(q, opt, mod):
      for x in iteration.keys:
          iteration.setter(x)

          begin = time()
          res = (method_obj(opt, mod))
          end = time()

          res.insert(0, (iteration.name, x))
          res.append(("Time", end - begin))
          q.put(res)
  
   res_t = []
   try: 
      queue = Queue()
      process = Process(target = wrap_exc, args = (G, queue, option_obj,model_obj))
      try:            
         process.start()

         begin = time()
         iterations = iteration.stepsNo
         for i in range(iterations):
           q = queue.get(timeout=begin + _timeout - time())
           if q[0][0] == "Exception":
               return q
           else:
              if res_t == []:
                  for k,v in q:
                      res_t.append((k,[v]))
              else:
                  idx = 0
                  for k,v in q:
                      assert res_t[idx][0] == k
                      res_t[idx][1].append(v)
                      idx = idx + 1
                          
      except Empty, exc:
         process.terminate()
         raise Exception("Method has worked more than " + str(_timeout) + "s. Please try another parameter combination")
                  
      process.join(timeout=_timeout)

   except Exception, exc:
      return [("Exception", str(exc))]#add_error(exc)
   return res_t

def _compute_iteration_2d(model_obj, option_obj, method_obj, iteration_1, iteration_2):

   def G(q, opt, mod):
      for x_1 in iteration_1.keys:
         for x_2 in iteration_2.keys:
            iteration_2.setter(x_2)
            iteration_1.setter(x_1)
            
            begin = time()
            res = (method_obj(opt, mod))
            end = time()
            res.append(("Time", end - begin))
            q.put((x_1, x_2, res))
   
   try: 
      keys = [
         (iteration_1.name, iteration_1.keys),
         (iteration_2.name, iteration_2.keys)
      ]
      data = []
      queue = Queue()
      process = Process(target = wrap_exc, args = (G, queue, option_obj, model_obj))
      try:            
         process.start()

         begin = time()
         iterations = iteration_1.stepsNo * iteration_2.stepsNo
         for i in range(iterations):
            x_1, x_2, q = queue.get(timeout=begin + _timeout - time())
            if len(data) == 0:
               for k,v in q:
                  data.append((k, [v]))
            else:
               idx = 0
               for k,v in q:
                   assert data[idx][0] == k
                   data[idx][1].append(v)
                   idx += 1

      except Empty, exc:
         process.terminate()
         raise Exception("Method has worked more than " + str(_timeout) + "s. Please try another parameter combination")
                  
      process.join(timeout=_timeout)

   except Exception, exc:
      return [("Exception", str(exc)+''.join(traceback.format_tb(sys.exc_info()[2])))]
   return keys + data

def compute(*args, **kwargs):
   [asset, model, model_params], [family, option, option_params], [method, method_params] = _parse(kwargs)

   iterables = []

   def append_iterables(label, setter, startValue, stopValue, interationsNo):
      iterables.append(Iterable(label, setter, startValue, stopValue, interationsNo))

   model_obj = _lookupModel(model).create(model_params, append_iterables)
   option_obj = _lookupOption(family, option).create(option_params, append_iterables)
   method_obj = _lookupMethod(model, family, method).create(method_params, append_iterables)

   
   result = _compute_scalar(model_obj, option_obj, method_obj) if iterables == [] else \
            _compute_iteration_1d(model_obj, option_obj, method_obj, iterables[0]) if len(iterables) == 1 else \
            _compute_iteration_2d(model_obj, option_obj, method_obj, iterables[0], iterables[1]) if len(iterables) == 2 else \
            []

   _return(result)

