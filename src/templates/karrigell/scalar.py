from HTMLTags import *

def load(v, property_name, vlabel, obj, conv):
    try:
       ctx = v.ctx
       if not ctx.history_mode and ctx.reload and vlabel in ctx.REQUEST:
          setattr(obj, property_name, conv(ctx.REQUEST[vlabel]))
    except Exception, ex:
       v.addError('Error in '+ property_name +':' + str(ex))
       
def render(v, table, colors, obj, propname, label, vlabel, constr, onChange, iterable, conv):

   ctx = v.ctx
   if not ctx.history_mode:
      mc = INPUT(name=vlabel,onchange=onChange,value=getattr(obj, propname))      
   else:
      mc = getattr(obj, propname)
      constr = ''
   
   v.clrinc()   
   table <= (TR(TD(label,align='right') + TD(mc)+TD(constr),bgcolor=v.currentColor)) 

def getIterables(ctx, table, colors, obj, propname, label, vlabel, constr, onChange, iterable, conv):

   if iterable:
      ctx._iterables.append(label)
      ctx._iterables_corr.append(vlabel)
      ctx._iterables_getter.append(lambda: getattr(obj, propname))
      ctx._iterables_setter.append(lambda x: setattr(obj, propname, x)) 
