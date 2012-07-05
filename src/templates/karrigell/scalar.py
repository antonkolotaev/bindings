from HTMLTags import *
from kspremia.field_base import *

class Scalar (FieldBase):
  
   def __init__(self, propertyName,
                     friendlyName,
                     fullName,
                     constraint,
                     onChange,
                     isIterable,
                     converter):
      super(Scalar,self).__init__(propertyName, friendlyName, fullName)
      self.constraint = constraint
      self.onChange = onChange
      self.iterable = isIterable
      self.fromString = converter

   def load(self, v):
       try:
          if not v.history_mode and v.reload and self.fullName in v.REQUEST:
             setattr(v.entity, self.propertyName, self.fromString(v.REQUEST[self.fullName]))
       except Exception, ex:
          v.addError('Error in '+ self.propertyName +':' + str(ex))
          
   def render(self, v):

      if not v.history_mode:
         mc = INPUT(name=self.fullName,onchange=self.onChange,value=getattr(v.entity, self.propertyName))  
         rc = self.constraint    
      else:
         mc = getattr(v.entity, self.propertyName)
         rc = ''
      
      v.row(self.friendlyName, mc, rc)

   def getIterables(self, v):
      ctx = v.ctx

      if self.iterable:
         ctx._iterables.append(self.friendlyName)
         ctx._iterables_corr.append(self.fullName)
         ctx._iterables_getter.append(lambda: getattr(v.entity, self.propertyName))
         ctx._iterables_setter.append(lambda x: setattr(v.entity, self.propertyName, x))

def f(): pass