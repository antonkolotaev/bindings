def processIteration(ctx, obj, entity, colors):

   G = globals()
   
   clrinc()
   iterate_vlabel = 'iterate_'+entity
   _iterate_vlabel = '_iterate_'+entity
   
   if _iterate_vlabel in G:
      iterate_idx = int(G[_iterate_vlabel])
   else:
      iterate_idx = 0

   old_idx = G['oldit'][entity]
   #print "old_idx=",old_idx, "iterate_idx=", iterate_idx
   if old_idx == iterate_idx and 'il' in REQUEST and iterate_idx <> 0:
      #print iterate_idx >= len(ctx.iterables_corr), G['_il'], ctx.iterables_corr, iterate_idx  
      if iterate_idx >= len(ctx.iterables_corr) or G['_il'] <> ctx.iterables_corr[iterate_idx]:
         if G['_il'] in ctx.iterables_corr: 
            iterate_idx = ctx.iterables_corr.index(G['_il'])
         else:
            iterate_idx = 0
         #print "==>", iterate_idx

   iterate_value_vlabel = iterate_vlabel + '_' + ctx.iterables_corr[iterate_idx]
      
   table <= (TR(TD(('Iterate'),align='right') + TD(enum_submit_mod(iterate_vlabel, ctx.iterables, ctx.iterables[iterate_idx]))+TD(),bgcolor=clr(colors,clridx)))
   
   if iterate_idx <> 0:
   
      clrinc()

      if iterate_value_vlabel in REQUEST:
         old_value = REQUEST[iterate_value_vlabel]
      else:
         old_value = ctx.iterables_getter[iterate_idx]()
         
      table <= (TR(TD(('Iterate To'),align='right') + TD(INPUT(name=iterate_value_vlabel,value=old_value))+TD(),bgcolor=clr(colors,clridx)))
      
      clrinc()      
      table <= (TR(TD(('#Iterations'),align='right') + TD(INPUT(name='iteration_steps',value=10))+TD(),bgcolor=clr(colors,clridx)))
      
      G['_iterate_' + entity] = str(iterate_idx)
      G['iterate_object'] = obj
      G['iterate_label'] = ctx.iterables_corr[iterate_idx] 
      G['iterate_name'] = ctx.iterables[iterate_idx]
      G['iteration_getter'] = ctx.iterables_getter[iterate_idx]
      G['iteration_setter'] = ctx.iterables_setter[iterate_idx]
      if iterate_value_vlabel in REQUEST:
         G['iterate_to'] = float(REQUEST[iterate_value_vlabel])  

