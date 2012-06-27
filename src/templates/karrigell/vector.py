from HTMLTags import *

def idxFullName(fullName, idx):
   return fullName + '[' + str(idx) + ']'

def render(v, table, colors, fullName, friendlyName, pmem):
   def mc(fullName, toShow):
      if not v.ctx.history_mode:
         return INPUT(name=fullName,value=toShow)
      else:
         return friendlyName
   def rc(s):
      if not v.ctx.history_mode:
         return s  
      else:
         return ''    
   v.clrinc()
   table <= TR(TD(friendlyName, align='right',rowspan=len(pmem)) + 
               TD(mc(idxFullName(fullName, 0),pmem[0])) + 
               TD(rc('R'),
               rowspan=len(pmem)),
               bgcolor=v.currentColor)
               
   table <= Sum(
                [TR(TD(mc(idxFullName(fullName, i),pmem[i])),
                     bgcolor=v.currentColor) for i in range(1,len(pmem))]
               )

def load(ctx, property_name, vlabel, pmem):
    try:
       for i in range(len(pmem)):
          src = idxFullName(vlabel, i)
          if src in ctx.REQUEST: 
             pmem[i] = float(ctx.REQUEST[src])
    except Exception, ex:
       ctx.addError('Error in' + property_name + ':' + str(ex))

def getIterables(ctx, table, colors, obj, propname, label, vlabel):

   pmem = getattr(obj, propname)

   for i in range(len(pmem)): 
      ctx._iterables.append(idxFullName(label, i))
      ctx._iterables_corr.append(vlabel + str(i))

   ctx._iterables_getter.extend(map(lambda x: (lambda: x), pmem))
   ctx._iterables_setter.extend(map(lambda i: (lambda z: pmem.__setitem__(i, z)), range(len(pmem))))

