import json

Include('../import.py')
import premia.assets

def _return(x):
   print json.dumps(x)
   
def assets():
   _return(map(lambda a: a.__name__, premia.assets.all()))
   
def models(a):
   asset = eval('premia.assets.'+a)
   _return(map(lambda m: m.__name__, asset.models()))
