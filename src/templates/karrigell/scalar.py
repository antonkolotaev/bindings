def loadScalar(ctx, property_name, vlabel, obj, conv):
    try:
       if ctx.reload and vlabel in REQUEST:
          setattr(obj, property_name, conv(REQUEST[vlabel]))
    except Exception, ex:
       add_error('Error in '+ property_name +':' + str(ex))

def processScalar(ctx, table, colors, obj, propname, label, vlabel, constr, onChange, iterable, conv):
   loadScalar(ctx, propname, vlabel, obj, conv)
   table <= rowinc(colors, label, INPUT(name=vlabel,onchange=onChange,value=getattr(obj, propname)), constr)
   if iterable:
      ctx.iterables.append(label)
      ctx.iterables_corr.append(vlabel)
      ctx.iterables_getter.append(lambda: getattr(obj, propname))
      ctx.iterables_setter.append(lambda x: setattr(obj, propname, x)) 

