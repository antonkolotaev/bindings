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

    pmem = getattr(v.entity, self.propertyName)
    mode = getattr(v.entity, self.propnameMode)

    L = SELECT(name = self.varnameType, onchange='submit();').from_list(['Constant','Array'])

    if mode == 0:
      L.select(value=0)
      mc = INPUT(name=self.varnameConst,value=pmem[0])
      v.spannedRows(self.friendlyName, [L,mc],'R')

    if mode == 1:
      def mc(i): 
          return INPUT(name=self.varnameIdx(i),value=pmem[i])
      L.select(value=1)
      v.spannedRows(self.friendlyName, [L]+map(mc, range(len(pmem))), 'R')

  def renderHistory(self, v):

    pmem = getattr(v.entity, self.propertyName)
    mode = getattr(v.entity, self.propnameMode)
     
    if mode == 0:
      v.row(self.friendlyName, pmem[0])

    if mode == 1:

      v.spannedRows(self.friendlyName, pmem)

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
