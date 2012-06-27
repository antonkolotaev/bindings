from HTMLTags import *

def load(v, property_name, vlabel, obj):
   pmem = getattr(obj, property_name)
  
   if vlabel + '__t' in v.ctx.REQUEST:
      if v.ctx.REQUEST[vlabel + '__t'] == '0':
         mode = 0
      else:
         mode = 1
   else:
      if all(map(lambda x: x == pmem[0], pmem)):
         mode = 0
      else:
         mode = 1
   setattr(obj, '__mode_'+property_name, mode)

   try:
       if vlabel + '__c' in v.ctx.REQUEST:
         val = float(v.ctx.REQUEST[vlabel + '__c'])
         for i in range(len(pmem)):                 
            pmem[i] = val
       else:
          for i in range(len(pmem)):
             src = vlabel + "[" + str(i) + "]"
             if src in v.ctx.REQUEST: 
                pmem[i] = float(v.ctx.REQUEST[src])
   except Exception, ex:
       v.addError('Error in' + property_name + ':' + str(ex))

def render(v, table, colors, obj, propname, label, vlabel):
   ctx = v.ctx
   pmem = getattr(obj, propname)
   v.clrinc()

   mode = getattr(obj, '__mode_'+propname)
   
   if not ctx.history_mode:
      L = SELECT(name = vlabel+ '__t', onchange='submit();').from_list(['Constant','Array'])
      
   if mode == 0:

      if not ctx.history_mode:
         L.select(value=0)
         table <= TR(TD(label, align='right',rowspan=2) + TD(L) + TD('',rowspan=2),bgcolor=v.currentColor)
         mc = INPUT(name=vlabel + '__c',value=pmem[0])
         table <= TR(TD(mc),bgcolor=v.currentColor)
      else:
         table <= TR(TD(label, align='right') + TD(pmem[0]) + TD(''),bgcolor=v.currentColor)

   if mode == 1:
   
      if not ctx.history_mode:   
         L.select(value=1)
         table <= TR(TD(label, align='right',rowspan=len(pmem)+1) + TD(L) + TD('R',rowspan=len(pmem)+1),bgcolor=v.currentColor)
         table <= Sum([TR(TD(INPUT(name=vlabel + '[' + str(i) + ']',value=pmem[i])),bgcolor=v.currentColor) for i in range(0,len(pmem))])
      else:
         def as_string(arr):
            s = "["
            for x in arr:
               s += str(x) + ";"
            return s + "]"
         table <= TR(TD(label, align='right') + TD(as_string(pmem)) + TD(''),bgcolor=v.currentColor)


def getIterables(v, table, colors, obj, propname, label, vlabel):

   ctx = v.ctx

   pmem = getattr(obj, propname)

   mode = getattr(obj, '__mode_'+propname)
      
   if mode == 0:
      ctx._iterables.append(label)
      ctx._iterables_corr.append(vlabel + '__c')
      ctx._iterables_getter.append(lambda: pmem[0])
      def F(x):
         for i in range(len(pmem)): pmem[i] = x          
      ctx._iterables_setter.append(F)

   if mode == 1:
      for i in range(len(pmem)): 
         ctx._iterables.append(label + '[' + str(i) + ']')
         ctx._iterables_corr.append(vlabel + str(i))
      ctx._iterables_getter.extend(map(lambda x: (lambda: x), pmem))
      ctx._iterables_setter.extend(map(lambda i: (lambda z: pmem.__setitem__(i, z)), range(len(pmem))))