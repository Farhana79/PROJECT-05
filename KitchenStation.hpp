#include "KitchenStation.hpp"
/**
     * Default Constructor
     * @post: Initializes an empty kitchen station with default values.
     */
KitchenStation::KitchenStation():dishes_({}),ingredients_stock_({}) {}
/**
     * Parameterized Constructor
     * @param station_name A string representing the station's name.
     * @post: Initializes a kitchen station with the given name.
     */
KitchenStation::KitchenStation(const std::string &station_name, const std::vector<Dish *>& dishes ,
                               const std::vector<Ingredient>& ingredients_stock):station_name_(station_name),dishes_(dishes), ingredients_stock_(ingredients_stock) {

}
/**
     * Destructor
     * @post: Deallocates all dynamically allocated dishes in the station.
     */
KitchenStation::~KitchenStation() {
    for(size_t i =0; i < dishes_.size(); i++){ // Deallocate each dynamically allocated Dish
        if (dishes_[i] != nullptr){
            delete dishes_[i];
            dishes_[i] = nullptr;
        }
    }
    dishes_.clear(); // Clear the vector to remove all elements
}

/**
     * Retrieves the name of the kitchen station.
     * @return: The name of the station.
     */
const std::string &KitchenStation::getName() const {
    return station_name_;
}

/**
 * Sets the name of the kitchen station.
 * @param name A string representing the new station name.
 * @post: Updates the station's name.
 */

void KitchenStation::setName(const std::string &stationName) {
    station_name_ = stationName;
}

/**
* Retrieves the list of dishes assigned to the kitchen station.
* @return A vector of pointers to Dish objects assigned to the station.
*/
std::vector<Dish *> &KitchenStation::getDishes(){
    return dishes_;
}

/**
* Retrieves the ingredient stock available at the kitchen station.
* @return A vector of Ingredient objects representing the station's ingredient stock.
*/
std::vector<Ingredient> &KitchenStation::getIngredientsStock() {
    return ingredients_stock_;
}

/**
* Assigns a dish to the station.
* @param dish A pointer to a Dish object.
* @post: Adds the dish to the station's list of dishes if not
already present.
* @return: True if the dish was added successfully; false
otherwise.
*/
bool KitchenStation::assignDishToStation(Dish *dish) {
    for (Dish * existingDish: dishes_){
        if (existingDish->getName() == dish->getName()){
            return false;
        }
    }

    dishes_.push_back(dish);
    return true;
}

/**
* Replenishes the station's ingredient stock.
* @param ingredient An Ingredient object.
* @post: Adds the ingredient to the station's stock or updates the
quantity if it already exists.
*/
void KitchenStation::replenishStationIngredients(Ingredient ingredient) {
    for (Ingredient& existingIngredient : ingredients_stock_) {
        if (existingIngredient.name == ingredient.name) {
            existingIngredient.quantity += ingredient.quantity; // If exists increase the quantity
            return;
        }
    }

    // Ingredient not found, so add it to the stock
    ingredients_stock_.push_back(ingredient);
}

/**
    * Checks if the station can complete an order for a specific dish.
    * @param dish_name A string representing the name of the dish.
    * @return: True if the station has the dish assigned and all
    required ingredients are in stock; false otherwise.
    */
bool KitchenStation::canCompleteOrder(const std::string &dish_name) {
    Dish * targetedDish = nullptr;
    for(Dish * dish: dishes_){
        if (dish->getName() == dish_name){
            targetedDish = dish;
            break;
        }
    }

    if (targetedDish == nullptr){
        return false; // If dish is not found
    }
    const std::vector<Ingredient> requiredIngredients = targetedDish->getIngredients();

    for(const Ingredient& requiredIngredient: requiredIngredients){
        bool found = false;
        for (const Ingredient& stockIngredient : ingredients_stock_) {
            if (stockIngredient.name == requiredIngredient.name) {
                if (stockIngredient.quantity >= requiredIngredient.required_quantity) {
                    found = true;
                    break;
                }
            }
        }
        if (!found){
            return false;
        }
    }
    // All required ingredients are in stock with sufficient quantities
    return true;
}

/**
    * Prepares a dish if possible.
    * @param dish_name A string representing the name of the dish.
    * @post: If the dish can be prepared, reduce the quantities of the
    used ingredients accordingly. If the stock ingredient is depleted to
    0, remove the ingredient from the Kitchen Station.
    * @return: True if the dish was prepared successfully; false otherwise
    */
bool KitchenStation::prepareDish(const std::string &dish_name) {
    // Check if the dish can be completed with available ingredients
    if (!canCompleteOrder(dish_name)) {
        return false;
    }

    // Find the dish by name
    Dish* targetDish = nullptr;
    for (Dish* dish : dishes_) {
        if (dish->getName() == dish_name) {  // Assuming Dish has a getName() method
            targetDish = dish;
            break;
        }
    }

    // If the dish is not found, return false
    if (!targetDish) {
        return false;
    }

    // Get the required ingredients for the dish
    const std::vector<Ingredient>& requiredIngredients = targetDish->getIngredients();

    // Update the stock ingredients
    for (const Ingredient& requiredIngredient : requiredIngredients) {
        for (auto it = ingredients_stock_.begin(); it != ingredients_stock_.end(); ++it) {
            if (it->name == requiredIngredient.name) {
                int newQuantity = it->quantity - requiredIngredient.required_quantity;
                if (newQuantity <= 0) {
                    // Remove the ingredient if it's depleted
                    it = ingredients_stock_.erase(it);
                } else {
                    // Update the quantity of the stock ingredient
                    it->quantity = newQuantity;
                    ++it;  // Move iterator forward only if not erasing
                }
                break;
            }
        }
    }

    // Dish was successfully prepared
    return true;
}



