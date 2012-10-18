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

function ScalarValue(value) {
    var self = this;
    self.value = ko.observable(value);

    self.hasIteration = ko.observable(false);
    self.iterateTo = ko.observable(value);
    self.iterationsCount = ko.observable(10);

    self.serialized = function() {
        return parseFloat(self.value());
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
        return [$.map(self.values, function (value, i) { return value.serialized(); })];
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
            return [$.map(self.vector, function (value, i) { return value.serialized(); })];
        else {
            var res = [];
            for (i = 0; i < self.vector.length; i++) 
                res.push(self.scalar.serialized());
            return [res];
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

function Clayton_Params() {
    return [new ScalarField("alpha", 0.1), new ScalarField("beta", -0.3)];
}

function Student_Params() {
    return [new ScalarField("gamma", 0.21)];
}

function Alpha_Params() {
    return [new ScalarField("delta", 0.21)];
}

function Bs1D_Params() {
    return [
        new VectorCompactField("Compact", [100, 100, 200]),
        new VectorField("Vector", [0, 0.2, 0.3, 0.5]),
        new EnumField("CopulaType", "Clayton", [ 
            ["Student", Student_Params()],
            ["Clayton", Clayton_Params()],
            ["Alpha", Alpha_Params()]
        ]),
        new ScalarField("Volatility", 0.2),
        new ScalarField("Spot", 100.0)
    ];
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

    self.params = Bs1D_Params();

    self.params_flattened = ko.computed(function() {
        return flatten(self.params);
    });

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

    self.scalarResultRaw = ko.observable([]);
    self.resultQuery = ko.observable("");

    self.scalarResult = ko.computed(function() {
        return self.resultQuery() == self.query() ? self.scalarResultRaw() : [];
    });
}

mv = new ModelView();

ko.applyBindings(mv);

$('#Compute').click(function() { 
    var my_query = mv.query();
    mv.scalarResultRaw([]);
    mv.resultQuery(my_query);
    $.getJSON('api.ks/compute?'+my_query, function (data) {
        console.log(data);
        if (my_query == mv.query())
            mv.scalarResultRaw(data);
    }); 
});

/*
// Class to represent a row in the seat reservations grid
function SeatReservation(parent, name, initialMeal) {
    var self = this;
    self.name = name;
    self.parent = parent;
    self.meal = ko.observable(initialMeal);
    self.iterable = ko.observable(false);
    self.iterable_ex = ko.computed({
        read: self.iterable,
        write : function (value) {
            self.parent.pushIterable(self, value);
        },
        owner : self
    });
    
    self.formattedPrice = ko.computed(function() {
        var price = self.meal().price;
        p = self.iterable() ? "*" : "-";
        return p + (price ? "$" + price.toFixed(2) : "None");
    });     
}

// Overall viewmodel for this screen, along with initial state
function ReservationsViewModel() {
    var self = this;

    // Non-editable catalog data - would come from the server
    self.availableMeals = [
        { mealName: "Standard (sandwich)", price: 0 },
        { mealName: "Premium (lobster)", price: 34.9545 },
        { mealName: "Ultimate (whole zebra)", price: 290 }
    ];    

    self.selected = [];

    self.pushIterable = function(element, checked) {
        if (checked == false) {
            element.iterable(false);
            self.selected.remove(element);
        } else {
            self.selected.push(element);
            while (self.selected.length > 2) {
                self.selected.shift().iterable(false);
            }
            element.iterable(true);
        }
    };

    // Editable data
    self.seats = ko.observableArray([
        new SeatReservation(self, "Steve", self.availableMeals[1]),
        new SeatReservation(self, "Bert", self.availableMeals[0])
    ]);
    
    self.totalSurcharge = ko.computed(function() {
        var total = 0;
        for (var i = 0; i < self.seats().length; i++)
            total += self.seats()[i].meal().price;
        return total;
    });    
    
    
    self.addSeat = function() {
        self.seats.push(new SeatReservation(self, "", self.availableMeals[0]));
    }
    
    
    self.removeSeat = function(seat) { self.seats.remove(seat) }
}

ko.applyBindings(new ReservationsViewModel());
*/