function get(query) {
   z = ($.ajax({
     url: 'api.ks/'+query,
     dataType: 'json',
     async: false
   }));

   return $.parseJSON(z.responseText);
}

function Models() {
   var self = this;
   self._storage = {};
   return function (asset) {
        if (self._storage[asset] == undefined)
            self._storage[asset] = get('models?a='+asset)
        return self._storage[asset];
   };  
}

function ModelView() {
    var self = this; 

    self.assets = get('assets');

    self.models = Models();

    self.myAsset = ko.observable(self.assets[0]);
    self.myModels = ko.computed(function(){
        return self.models(self.myAsset());
    });
    self.myModel = ko.observable(self.myModels()[0]);
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