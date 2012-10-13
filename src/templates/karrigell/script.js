function get(query) {
   z = ($.ajax({
     url: 'api.ks/'+query,
     dataType: 'json',
     async: false
   }));

   return $.parseJSON(z.responseText);
}

function CachedMap(query_fun) {
   var self = this;
   self._storage = {};
   return function (key) {
        if (self._storage[key] == undefined)
            self._storage[key] = get(query_fun(key));
        return self._storage[key];
   };  
}

function ScalarField(label, value) {
    var self = this;
    self.label = label;
    self.value = ko.observable(value);
    self.renderer = 'scalar-row-template';
    
    self.getFields = function() {
        return [self];
    }
}

function VectorField(label, values) {
    var self = this;
    self.label = label;
    self.renderer = 'vector-row-template';
    self.values = $.map(values, function (value) {
        return ko.observable(value);
    });
    self.spansize = function () {
        return self.values.length;
    }
    self.getFields = function() {
        return [self];
    }
}

function VectorCompactField(label, isvector, values) {
    var self = this;
    self.label = label;
    self.renderer = 'vectorcompact-row-template';
    self.options = ["Constant", "Array"];
    self.isvector = ko.observable(isvector ? "Array" : "Constant");
    self.spansize = function () {
        return 1 + (self.isvector() == "Array" ? self.vector.length : 1);
    }
    self.scalar = ko.observable(values[0]);
    self.vector = $.map(values, function (value) {
        return ko.observable(value);
    });

    self.elements = ko.computed(function() {
        return self.isvector() == "Array" ? self.vector : [self.scalar];
    });

    self.getFields = function() {
        return [self];
    }
}


function iterkeys(d) {
    acc = [];    
    for (e in d) {
        acc.push(e);
    }
    return acc;
}

function flatten(arr){
    return arr.reduce(function(acc, val){
        return acc.concat(val.getFields());
    },[]);
}


function EnumField(label, value, options) {
    var self = this;
    self.label = label;
    self.value = ko.observable(value);
    self.options = iterkeys(options);
    self.renderer = 'enum-row-template';
    
    self.getFields = function() {
        return [self].concat(flatten(options[self.value()]));
    }
}

function Clayton_Params() {
    return [new ScalarField("alpha", 0.1), new ScalarField("beta", -0.3)];
}

function Student_Params() {
    return [new ScalarField("gamma", 0.21)];
}

function Bs1D_Params() {
    return [
        new VectorCompactField("Compact", false, [100, 100, 200]),
        new VectorField("Vector", [0, 0.2, 0.3, 0.5]),
        new EnumField("CopulaType", "Clayton", { 
            "Clayton" : Clayton_Params(),
            "Student" : Student_Params()
        }),
        new ScalarField("Volatility", 0.2),
        new ScalarField("Spot", 100.0)
    ];
}


function ModelView() {
    var self = this; 

    self.params = Bs1D_Params();

    self.params_flattened = ko.computed(function() {
        return flatten(self.params);
    });

    self.assets = get('assets');

    self.models = CachedMap(function(asset) {return 'models?a='+asset; });;
    self.families = CachedMap(function(model) {return 'families?m='+model; });;
    self.options = CachedMap(function(args){ return 'options?m='+args[0]+"&f="+args[1];});;
    self.methods = CachedMap(function(args){ return 'methods?m='+args[0]+"&f="+args[1]+"&o="+args[2];});

    self.myAsset = ko.observable(self.assets[0]);
    self.myModels = ko.computed(function(){
        return self.models(self.myAsset());
    });
    self.myModel = ko.observable(self.myModels()[0]);

    self.myFamilies = ko.computed(function(){
        return self.families(self.myModel());
    });
    self.myFamily = ko.observable(self.myFamilies()[0]);

    self.myOptions = ko.computed(function(){
        return self.options([self.myModel(), self.myFamily()]);
    });
    self.myOption = ko.observable(self.myOptions()[0]);
    
    self.myMethods = ko.computed(function(){
        return self.methods([self.myModel(), self.myFamily(), self.myOption()]);
    });
    self.myMethod = ko.observable(self.myMethods()[0]);    
}

ko.applyBindings(new ModelView());


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