from HTMLTags import *

def js_include(filename):
   return SCRIPT(type="text/javascript", src=filename)

print """
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
   <link rel="stylesheet" type="text/css" href="style.css" />
   <script type="text/javascript" src="jquery-1.8.2.min.js"></script>
   <script type="text/javascript" src="knockout-2.1.0.js"></script>
</head>
"""

Include("param_table.html")
Include("current.html")

print """


<table>
    <tbody data-bind="template: { name: 'row-template', foreach: params_flattened }">
    </tbody>
</table>
"""

print js_include("script.js")
