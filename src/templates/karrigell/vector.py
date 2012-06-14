def renderVector(table, colors, label, vlabel, pmem):
   def mc(vlabel, label):
      if not history_mode:
         return INPUT(name=vlabel,value=label)
      else:
         return label
   def rc(s):
      if not history_mode:
         return s  
      else:
         return ''    
   clrinc()
   table <= TR(TD(label, align='right',rowspan=len(pmem)) + TD(mc(vlabel + '[0]',pmem[0])) + TD(rc('R'),rowspan=len(pmem)),bgcolor=clr(colors,clridx))
   table <= Sum([TR(TD(mc(vlabel + '[' + str(i) + ']',pmem[i])),bgcolor=clr(colors,clridx)) for i in range(1,len(pmem))])

def iterVector(ctx, pmem, label, vlabel):
   
   for i in range(len(pmem)): 
      ctx.iterables.append(label + '[' + str(i) + ']')
      ctx.iterables_corr.append(vlabel + str(i))

   ctx.iterables_getter.extend(map(lambda x: (lambda: x), pmem))
   ctx.iterables_setter.extend(map(lambda i: (lambda z: pmem.__setitem__(i, z)), range(len(pmem))))

def loadVector(property_name, vlabel, pmem):
    try:
       for i in range(len(pmem)):
          src = vlabel + "[" + str(i) + "]"
          if src in REQUEST: 
             pmem[i] = float(REQUEST[src])
    except Exception, ex:
       add_error('Error in' + property_name + ':' + str(ex))

def processVector(ctx, table, colors, obj, propname, label, vlabel):
   pmem = getattr(obj, propname)
   #if ctx.reload and not history_mode:
   #   loadVector(propname, vlabel, pmem)
   #renderVector(table, colors, label, vlabel, pmem)
   iterVector(ctx, pmem, label, vlabel)

