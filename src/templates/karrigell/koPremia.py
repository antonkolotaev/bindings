from HTMLTags import *

def js_include(filename):
   return SCRIPT(type="text/javascript", src=filename)

print HEAD(Sum([ \
   js_include("jquery-1.8.2.min.js"),
   js_include("knockout-2.1.0.js"),
   ]))

Include("param_table.html")
Include("current.html")

print """


<table>
    <tbody data-bind="template: { name: 'row-template', foreach: params_flattened }">
    </tbody>
</table>
"""

print js_include("script.js")
