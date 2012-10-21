function get(query) {
   z = ($.ajax({
     url: 'api.ks/'+query,
     dataType: 'json',
     async: false
   }));

   return $.parseJSON(z.responseText);
}

function CachedMap(getter) {
   var self = this;
   self._storage = {};
   self.at = function (key) {
        if (self._storage[key] == undefined){
            //console.log(key + ' not found -> fetching');
            self._storage[key] = getter(key);
        } //else console.log(key + 'found');
        return self._storage[key];
   };  
}

function KsCachedMap(query_fun) {
    return new CachedMap(function (key) {
        return get(query_fun(key));
    });
}

function ParamCachedMap(query_fun) {
    return new CachedMap(function (key) {
            return loadParams(get(query_fun(key)));
        });
}

var iterables = ko.observableArray([]);

function ScalarValue(value) {
    var self = this;
    self.value = ko.observable(value);

    self.iterateTo = ko.observable(value);
    self.iterationsCount = ko.observable(10);

    self.hasIteration = ko.computed({
        read: function () { return iterables().indexOf(self) != -1; },
        write: function (value) {
            if (value) {
                iterables.push(self);
                if (iterables().length > 2)
                    iterables.splice(0,1);
            } else {
                iterables.splice(iterables.indexOf(self), 1);
            }
        }
    })

    self.serialized = function() {
        var x = parseFloat(self.value());
        return self.hasIteration() ? ['iterate', x, parseFloat(self.iterateTo()), parseFloat(self.iterationsCount())] : x;
    }
}

function ScalarField(label, value) {
    var self = this;
    self.label = label;
    self.value = new ScalarValue(value);
    self.renderer = 'scalar-row-template';

    self.getFields = function() {
        return [self];
    }

    self.serialized = function() {
        return [self.value.serialized()];
    }

}

function map(elements, f) {
    var res = [];
    for (var i=0; i<elements.length; i++)
        res.push(f(elements[i], i));
    return res;
}

function VectorField(label, values) {
    var self = this;
    self.label = label;
    self.renderer = 'vector-row-template';

    self.values = $.map(values, function (value, i) { return new ScalarValue(value); });

    self.spansize = function () {
        return self.values.reduce(function(acc, val){
            return acc + (val.hasIteration() ? 2 : 1);
        },0);
    }
    self.getFields = function() {
        return [self];
    }

    self.serialized = function() {
        return [map(self.values, function (value) { return value.serialized(); })];
    }

}

function FilenameField(label, value) {
    var self = this;
    self.label = label;
    self.value = value;
    self.renderer = 'filename-row-template';
    self.getFields = function () { return []; }
    self.serialized = function () { return [self.value]; }
}

function allequal(arr) {
    var a = arr[0];
    for (i = 1; i < arr.length; i++)
        if (a != arr[i])
            return false;
    return true;
}

function VectorCompactField(label, values) {
    var self = this;
    var isvector = allequal(values) == false;
    self.label = label;
    self.renderer = 'vectorcompact-row-template';
    self.options = ["Constant", "Array"];
    self.isvector = ko.observable(isvector ? "Array" : "Constant");
    self.spansize = function () {
        return 1 + self.elements().reduce(function(acc, val){
            return acc + (val.hasIteration() ? 2 : 1);
        },0);
    }
    self.vector = $.map(values, function (value) {
        return new ScalarValue(value);
    });
    self.scalar = self.vector[0];

    self.elements = ko.computed(function() {
        return self.isvector() == "Array" ? self.vector : [self.scalar];
    });

    self.at = function(i) {
        return self.elements()[i];
    }

    self.getFields = function() {
        return [self];
    }

    self.serialized = function() {
        if (self.isvector() == "Array")
            return [map(self.vector, function (value) { return value.serialized(); })];
        else {
            return [[self.scalar.serialized()]];
        }
    }
}


function iterkeys(arr) {
    acc = [];    
    for (i = 0; i < arr.length; i++)
        acc.push(arr[i][0]);
    return acc;
}

function lookup(arr, what) {
    for (i = 0; i < arr.length; i++)
        if (arr[i][0] == what)
            return arr[i][1];
    console.log('failed to find ' + what + ' in ' + arr);
    return undefined;
}

function flatten(arr){
    return arr.reduce(function(acc, val){
        return acc.concat(val.getFields());
    },[]);
}

function serialized(arr){
    return arr.reduce(function(acc, val){
        return acc.concat(val.serialized());
    },[]);
}

function EnumField(label, value, options) {
    // console.log(options)
    var self = this;
    self.label = label;
    self.value = ko.observable(value);
    self.options = iterkeys(options);
    self.renderer = 'enum-row-template';

    self.getFields = function() {
        return [self].concat(flatten(lookup(options, self.value())));
    }

    self.serialized = function() {
        return [[self.value(), serialized(lookup(options, self.value()))]];
    }
}

var enum_params = new CachedMap(function (e) { 
    var response = get('enum_params?e='+e);
    var res = [];
    for (i = 0; i < response.length; i++)
        res.push([response[i][0], loadParams(response[i][1])]);
    return res;
});

var assets = get('assets');

var models = KsCachedMap(function(asset) {return 'models?a='+asset; });;
var families = KsCachedMap(function(model) {return 'families?m='+model; });;
var options = KsCachedMap(function(args){ return 'options?m='+args[0]+"&f="+args[1];});;
var methods = KsCachedMap(function(args){ return 'methods?m='+args[0]+"&f="+args[1]+"&o="+args[2];});

var model_params = ParamCachedMap(function (model) { return 'model_params?m='+model; });
var option_params = ParamCachedMap(function (args) { return 'option_params?f='+args[0]+"&o="+args[1]; });
var method_params = ParamCachedMap(function (args) { return 'method_params?m='+args[0]+"&f="+args[1]+"&meth="+args[2]; });

function loadParams(raw) {
    return $.map(raw, function (e) {
        return (
            (e[1] == 0) ? new ScalarField(e[0], e[2]) :
            (e[1] == 1) ? new VectorField(e[0], e[2]) :
            (e[1] == 2) ? new VectorCompactField(e[0], e[2]) :
            (e[1] == 3) ? new FilenameField(e[0], e[2]) :
            new EnumField(e[0], e[2], enum_params.at(e[1])));
    });
}

function ModelView() {
    var self = this; 

    //------------------------------------------------------- Asset

    self.myAsset = ko.observable(assets[0]);
    self.myAssetProxy = ko.computed({
        read: function () { return self.myAsset() },
        write: function (value) { 
            self.myAsset(value); 
        }
    });

    //-------------------------------------------------------- Model

    self.myModels = ko.computed(function(){
        return models.at(self.myAsset());
    });
    
    self.myModel = ko.observable(self.myModels()[0]);

    self.myModelProxy = ko.computed({
        read: function () { return self.myModel() },
        write: function (value) { 
            self.myModel(value); 
        }
    });

    self.myModelParamsNF = ko.computed(function () {
        return model_params.at(self.myModel());
    });

    self.myModelParams = ko.computed(function () {
        return flatten(self.myModelParamsNF());
    });

    //---------------------------------------------------------- Family

    self.myFamilies = ko.computed(function(){
        return families.at(self.myModel());
    });

    self.myFamily = ko.observable(self.myFamilies()[0]);
    self.myFamilyProxy = ko.computed({
        read: function () { return self.myFamily() },
        write: function (value) { 
            self.myFamily(value); 
        }
    });

    //------------------------------------------------------------ Option

    self.myOptions = ko.computed(function(){
        return options.at([self.myModel(), self.myFamily()]);
    });
    self.myOption = ko.observable(self.myOptions()[0]);
    self.myOptionProxy = ko.computed({
        read: function () { return self.myOption() },
        write: function (value) { 
            self.myOption(value); 
        }
    });
    
    self.myOptionParamsNF = ko.computed(function () {
        return option_params.at([self.myFamily(), self.myOption()]);
    });
    self.myOptionParams = ko.computed(function () {
        return flatten(self.myOptionParamsNF());
    });

    //--------------------------------------------------------------- Method

    self.myMethods = ko.computed(function(){
        return methods.at([self.myModel(), self.myFamily(), self.myOption()]);
    });
    self.myMethod = ko.observable(self.myMethods()[0]);    
    self.myMethodProxy = ko.computed({
        read: function () { return self.myMethod() },
        write: function (value) { 
            //self.resultIsRelevant(false);
            self.myMethod(value); 
        }
    });

    self.myMethodParamsNF = ko.computed(function () {
        return method_params.at([self.myModel(), self.myFamily(), self.myMethod()]);
    });

    self.myMethodParams = ko.computed(function () {
        return flatten(self.myMethodParamsNF());
    });

    //------------------------------------------------------------------- Result

    self.query = ko.computed(function () {
        return $.toJSON([
                [self.myAsset(), self.myModel(), serialized(self.myModelParamsNF())],
                [self.myFamily(), self.myOption(), serialized(self.myOptionParamsNF())],
                [self.myMethod(), serialized(self.myMethodParamsNF())]
            ]);
    });

    self.iterationRang = ko.computed(function() {
        return iterables().length;
    })

    self.resultRaw = ko.observable([]);
    self.resultQuery = ko.observable("");

    self.scalarResult = ko.computed(function() {
        return (self.iterationRang() == 0 && self.resultQuery() == self.query() 
            ? self.resultRaw() 
            : []);
    });

    self.iterationResult1d = ko.computed(function() {
        return (self.iterationRang() == 1 && self.resultQuery() == self.query() 
            ? self.resultRaw() 
            : []); 
    })  

    self.iteration1dGraphsData = ko.computed(function() {
        var graphs = [];
        var src = self.iterationResult1d();
        for (var i = 1; i<src.length; i++) {
            var s = src[i];
            graphs.push({
                data: map(s[1], function(x,j) { return [src[0][1][j], x]; }),
                label: s[0]
            });
        }
        return graphs;
    })

    self.renderGraph1d = function (elem, data) {
        //console.log("elem="+elem+",data="+data.label+", type="+map(elem, function (e) {return e.nodeType; }));
        for (var i=0; i<elem.length; i++) {
            var e = elem[i];
            if (e.nodeType==1) {
                Flotr.draw(e, [data], {
                    legend : {
                        position : 'se',            // Position the legend 'south-east'.
                        backgroundColor : '#D2E8FF' // A light blue background color.
                    },
                    HtmlText : false
                });
            }
        }
    } 
}

mv = new ModelView();

ko.applyBindings(mv);

$('#Compute').click(function() { 
    var my_query = mv.query();
    mv.resultRaw([]);
    mv.resultQuery(my_query);
    $.getJSON('api.ks/compute?'+my_query, function (data) {
        console.log(data);
        if (my_query == mv.query())
            mv.resultRaw(data);
    }); 
});
