import json

Include('../import.py')
import premia.assets

def _return(x):
   print json.dumps(x)
   
def lookupAsset(a):
   return eval('premia.assets.'+a)
   
def lookupModel(m):
   return eval('premia.models.'+m)
   
def lookupOption(f, o):
   return eval('premia.options.'+f+'.'+o)
   
def assets():
   _return(map(lambda a: a.__name__, premia.assets.all()))
   
def models(a):
   _return(map(lambda m: m.__name__, lookupAsset(a).models()))
   
def families(m):
   _return(lookupModel(m).families())

def options(m, f):
   m_id = lookupModel(m).ID()
   opts = eval('pricings.' + m_id + '.' + m_id + '_' + f + '.methods_for_options()').keys()
   _return(map(lambda o: o.__name__, opts))
   
def methods(m, f, o):
   m_id = lookupModel(m).ID()
   opt = lookupOption(f, o)
   methods = [x.__name__ for x in eval('pricings.' + m_id + '.' + m_id + '_' + f + '.all()') if opt in x.options()]
   _return(methods)
   