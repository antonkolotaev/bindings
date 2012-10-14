import json

Include('../import.py')
import premia.assets

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
