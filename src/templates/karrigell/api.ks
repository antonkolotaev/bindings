import json
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
      self.label = label
      self.setter = setter

_timeout = 30

def _compute_scalar(model_obj, option_obj, method_obj):
   res = []
   def F(opt, mod, q):
      begin = time()
      try:
         res = method_obj(opt, mod)
      except Exception, exc:
         print "Caught:", exc
         res = [("Exception", str(exc))]
         q.put(res)
         return res
      end = time()
      res.append(("Time", end - begin))
      q.put(res)
   try:
      queue = Queue()
      process = Process(target = F, args = (option_obj,model_obj, queue))
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



def compute(*args, **kwargs):
   [asset, model, model_params], [family, option, option_params], [method, method_params] = _parse(kwargs)

   #_return([[asset, model, model_params], [family, option, option_params], [method, method_params]])

   iterables = []

   def append_iterables(label, setter, startValue, stopValue, interationsNo):
      iterables.append(Iterable(label, setter, startValue, stopValue, interationsNo))

   model_obj = _lookupModel(model).create(model_params, append_iterables)
   option_obj = _lookupOption(family, option).create(option_params, append_iterables)
   method_obj = _lookupMethod(model, family, method).create(method_params, append_iterables)

   result = _compute_scalar(model_obj, option_obj, method_obj)
   for k in iterables:
      result.append((k.label, k.keys))

   _return(result)

