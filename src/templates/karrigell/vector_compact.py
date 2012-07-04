from HTMLTags import *
from kspremia.field_base import *

class VectorCompact(FieldBase):

  def __init__(self,
               propertyName,
               friendlyName,
               fullName):

    super(VectorCompact, self).__init__(propertyName, friendlyName, fullName)
    self.varnameType = self.fullName + '__t'
    self.varnameConst = self.fullName + '__c'
    self.propnameMode = '__mode_'+self.propertyName

  def varnameIdx(self, i):
      return brackets(self.fullName, i)

  def load(self, v):

     if v.reload and not v.history_mode:

       pmem = getattr(v.entity, self.propertyName)
      
       if self.varnameType in v.REQUEST:
          if v.REQUEST[self.varnameType] == '0':
             mode = 0
          else:
             mode = 1
       else:
          if all(map(lambda x: x == pmem[0], pmem)):
             mode = 0
          else:
             mode = 1
       setattr(v.entity, self.propnameMode, mode)

       try:
           if self.varnameConst in v.REQUEST:
             val = float(v.REQUEST[self.varnameConst])
             for i in range(len(pmem)):                 
                pmem[i] = val
           else:
              for i in range(len(pmem)):
                 src = self.varnameIdx(i)
                 if src in v.REQUEST: 
                    pmem[i] = float(v.REQUEST[src])
       except Exception, ex:
           v.addError('Error in' + self.propertyName + ':' + str(ex))

  def render(self, v):

     ctx = v.ctx
     pmem = getattr(v.entity, self.propertyName)
     v.clrinc()

     mode = getattr(v.entity, self.propnameMode)
     
     if not v.history_mode:
        L = SELECT(name = self.varnameType, onchange='submit();').from_list(['Constant','Array'])
        
     if mode == 0:

        if not v.history_mode:
           L.select(value=0)
           v.table <= TR(TD(self.friendlyName, align='right',rowspan=2) + TD(L) + TD('',rowspan=2),bgcolor=v.currentColor)
           mc = INPUT(name=self.varnameConst,value=pmem[0])
           v.table <= TR(TD(mc),bgcolor=v.currentColor)
        else:
           v.table <= TR(TD(self.friendlyName, align='right') + TD(pmem[0]) + TD(''),bgcolor=v.currentColor)

     if mode == 1:
     
        if not ctx.history_mode:   
           L.select(value=1)
           v.table <= TR(TD(self.friendlyName, align='right',rowspan=len(pmem)+1) + TD(L) + TD('R',rowspan=len(pmem)+1),bgcolor=v.currentColor)
           v.table <= Sum([TR(TD(INPUT(name=self.varnameIdx(i),value=pmem[i])),bgcolor=v.currentColor) for i in range(0,len(pmem))])
        else:
           def as_string(arr):
              s = "["
              for x in arr:
                 s += str(x) + ";"
              return s + "]"
           v.table <= TR(TD(self.friendlyName, align='right') + TD(as_string(pmem)) + TD(''),bgcolor=v.currentColor)


  def getIterables(self, v):

     ctx = v.ctx

     pmem = getattr(v.entity, self.propertyName)

     mode = getattr(v.entity, self.propnameMode)
        
     if mode == 0:
        ctx._iterables.append(self.friendlyName)
        ctx._iterables_corr.append(self.varnameConst)
        ctx._iterables_getter.append(lambda: pmem[0])
        def F(x):
           for i in range(len(pmem)): pmem[i] = x          
        ctx._iterables_setter.append(F)

     if mode == 1:
        for i in range(len(pmem)): 
           ctx._iterables.append(self.varnameIdx(i))
           ctx._iterables_corr.append(self.fullName + str(i))
        ctx._iterables_getter.extend(map(lambda x: (lambda: x), pmem))
        ctx._iterables_setter.extend(map(lambda i: (lambda z: pmem.__setitem__(i, z)), range(len(pmem))))

def f(): pass
