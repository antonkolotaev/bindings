from HTMLTags import *

class Scalar (object):
  
   def __init__(self, correctedName,
                     printableName,
                     fullName,
                     constraint,
                     onChange,
                     isIterable,
                     converter):
      self.correctedName = correctedName
      self.printableName = printableName
      self.fullName = fullName
      self.constraint = constraint
      self.onChange = onChange
      self.iterable = isIterable
      self.fromString = converter


   def load(self, v):
       try:
          if not v.history_mode and v.reload and self.fullName in v.REQUEST:
             setattr(v.entity, self.correctedName, self.fromString(v.REQUEST[self.fullName]))
       except Exception, ex:
          v.addError('Error in '+ self.correctedName +':' + str(ex))
          
   def render(self, v):

      ctx = v.ctx
      if not v.history_mode:
         mc = INPUT(name=self.fullName,onchange=self.onChange,value=getattr(v.entity, self.correctedName))      
      else:
         mc = getattr(v.entity, self.correctedName)
      
      v.clrinc()   
      v.table <= (TR(
                     TD(self.printableName,align='right') + 
                     TD(mc)+
                     TD(self.constraint),
                     bgcolor=v.currentColor
                    )) 

   def getIterables(self, v):
      ctx = v.ctx

      if self.iterable:
         ctx._iterables.append(self.printableName)
         ctx._iterables_corr.append(self.fullName)
         ctx._iterables_getter.append(lambda: getattr(v.entity, self.correctedName))
         ctx._iterables_setter.append(lambda x: setattr(v.entity, self.correctedName, x))

def f(): pass