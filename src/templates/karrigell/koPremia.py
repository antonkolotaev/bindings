from HTMLTags import *

def js_include(filename):
   return SCRIPT(type="text/javascript", src=filename)

print HEAD(Sum([ \
   js_include("jquery-1.8.2.min.js"),
   js_include("knockout-2.1.0.js"),
   ]))

print """
<table>
    <select data-bind="options: assets, value: myAsset"></select>
    <select data-bind="options: myModels, value: myModel"></select>
</table>
"""

print js_include("script.js")


