from HTMLTags import *
from kspremia.field_base import *

def brackets(s, i):
  return s + '[' + str(i) + ']'

class Vector(FieldBase):

  def __init__(self,
               propertyName,
               friendlyName,
               fullName):
    super(Vector,self).__init__(propertyName,friendlyName,fullName)

  def render(self, v):

    pmem = getattr(v.entity, self.propertyName)

    def mc(idx):
      fullName = brackets(self.fullName, idx)
      toShow = pmem[idx]
      if not v.history_mode:
         return INPUT(name=fullName,value=toShow)
      else:
         return toShow
    def rc(s):
      if not v.history_mode:
         return s  
      else:
         return ''    
    v.clrinc()
    v.table <= TR(TD(self.friendlyName, align='right',rowspan=len(pmem)) + 
                  TD(mc(0)) + 
                  TD(rc('R'),
                  rowspan=len(pmem)),
                  bgcolor=v.currentColor)
               
    v.table <= Sum(
                   [TR(TD(mc(i)),
                     bgcolor=v.currentColor) for i in range(1,len(pmem))]
                  )

  def load(self, v):

      pmem = getattr(v.entity, self.propertyName)
      if v.reload and not v.history_mode:
        try:
           for i in range(len(pmem)):
              src = brackets(self.fullName, i)
              if src in v.REQUEST: 
                 pmem[i] = float(v.REQUEST[src])
        except Exception, ex:
           v.addError('Error in' + self.propertyName + ':' + str(ex))

  def getIterables(self, v):

    ctx = v.ctx

    pmem = getattr(v.entity, self.propertyName)

    for i in range(len(pmem)): 
      ctx._iterables.append(brackets(self.friendlyName, i))
      ctx._iterables_corr.append(self.fullName + str(i))

    ctx._iterables_getter.extend(map(lambda x: (lambda: x), pmem))
    ctx._iterables_setter.extend(map(lambda i: (lambda z: pmem.__setitem__(i, z)), range(len(pmem))))

def f(): pass