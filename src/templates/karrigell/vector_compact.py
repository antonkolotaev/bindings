def loadVectorCompact(property_name, vlabel, pmem):
    try:
       if vlabel + '__c' in REQUEST:
         val = float(REQUEST[vlabel + '__c'])
         for i in range(len(pmem)):                 
            pmem[i] = val
       else:
          for i in range(len(pmem)):
             src = vlabel + "[" + str(i) + "]"
             if src in REQUEST: 
                pmem[i] = float(REQUEST[src])
    except Exception, ex:
       add_error('Error in' + property_name + ':' + str(ex))

def processVectorCompact(ctx, table, colors, obj, propname, label, vlabel):
   pmem = getattr(obj, propname)
   clrinc()
   mode = None
   
   if ctx.reload and not history_mode:
      loadVectorCompact(propname, vlabel, pmem)

   if vlabel + '__t' in REQUEST:
      if REQUEST[vlabel + '__t'] == '0':
         mode = 0
      else:
         mode = 1
   else:
      if all(map(lambda x: x == pmem[0], pmem)):
         mode = 0
      else:
         mode = 1

   if not history_mode:
      L = SELECT(name = vlabel+ '__t', onchange='submit();').from_list(['Constant','Array'])
      
   if mode == 0:
      ctx.iterables.append(label)
      ctx.iterables_corr.append(vlabel + '__c')
      ctx.iterables_getter.append(lambda: pmem[0])
      def F(x):
         for i in range(len(pmem)): pmem[i] = x          
      ctx.iterables_setter.append(F)

      if not history_mode:
         L.select(value=0)
         table <= TR(TD(label, align='right',rowspan=2) + TD(L) + TD('',rowspan=2),bgcolor=clr(colors,clridx))
         mc = INPUT(name=vlabel + '__c',value=pmem[0])
         table <= TR(TD(mc),bgcolor=clr(colors,clridx))
      else:
         table <= TR(TD(label, align='right') + TD(pmem[0]) + TD(''),bgcolor=clr(colors,clridx))
            
      

   if mode == 1:
      for i in range(len(pmem)): 
         ctx.iterables.append(label + '[' + str(i) + ']')
         ctx.iterables_corr.append(vlabel + str(i))
      ctx.iterables_getter.extend(map(lambda x: (lambda: x), pmem))
      ctx.iterables_setter.extend(map(lambda i: (lambda z: pmem.__setitem__(i, z)), range(len(pmem))))
   
      if not history_mode:   
         L.select(value=1)
         table <= TR(TD(label, align='right',rowspan=len(pmem)+1) + TD(L) + TD('R',rowspan=len(pmem)+1),bgcolor=clr(colors,clridx))
         table <= Sum([TR(TD(INPUT(name=vlabel + '[' + str(i) + ']',value=pmem[i])),bgcolor=clr(colors,clridx)) for i in range(0,len(pmem))])
      else:
         def as_string(arr):
            s = "["
            for x in arr:
               s += x
               s += ";"
            return s + "]"
         table <= TR(TD(label, align='right') + TD(as_string(pmem)) + TD(''),bgcolor=clr(colors,clridx))

