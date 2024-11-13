#include "StationManager.hpp"

/**
    * Default Constructor
    * @post: Initializes an empty station manager.
    */
StationManager::StationManager():LinkedList<KitchenStation *>() {}

StationManager::~StationManager() {
    Node<KitchenStation *>* current = nullptr;

    for (int i = 0; i < item_count_; ++i) {
        current = getNodeAt(i);
        delete current->getItem();
    }

}

/**
    * Adds a new station to the station manager.
    * @param station A pointer to a KitchenStation object.
    * @post: Inserts the station into the linked list.
    * @return: True if the station was successfully added; false
    otherwise.
    */
bool StationManager::addStation(KitchenStation *station) {
    return insert(0, station);
}

/**
    * Removes a station from the station manager by name.
    * @param station_name A string representing the station's name.
    * @post: Removes the station from the list and deallocates it.
    * @return: True if the station was found and removed; false
    otherwise.
    */
bool StationManager::removeStation(const std::string &station_name) {
    int i;
    int foundIndex = -1;
    bool found = false;
    for (i = 0; i < item_count_; ++i) {
        KitchenStation* kitchen_station_ptr = getEntry(i);
        if (kitchen_station_ptr->getName() == station_name){
            found = true;
            foundIndex = i;
            delete kitchen_station_ptr;
            break;
        }
    }
    if (found){
        return remove(foundIndex);
    }
    return false; // Not found
}

/**
    * Finds a station in the station manager by name.
    * @param station_name A string representing the station's name.
    * @return: A pointer to the KitchenStation if found; nullptr
    otherwise.
    */
KitchenStation *StationManager::findStation(const std::string &station_name) {
    int i;
    for (i = 0; i < item_count_; ++i) {
        KitchenStation* kitchen_station_ptr = getEntry(i);
        if (kitchen_station_ptr->getName() == station_name){
            return kitchen_station_ptr;
        }
    }

    return nullptr; // Not found
}

/**
    * Moves a specified station to the front of the station manager
    list.
    * @param station_name A string representing the station's name.
    * @post: The station is moved to the front of the list if it
    exists.
    * @return: True if the station was found and moved; false
    otherwise.
    */
bool StationManager::moveStationToFront(const std::string &station_name) {
    for (int i = 0; i < item_count_; ++i) {
        Node<KitchenStation*>* current = getNodeAt(i);
        if ( current->getItem()->getName() == station_name && i > 0){
            Node<KitchenStation*>* prev = getNodeAt(i-1);
            prev->setNext(current->getNext());
            current->setNext(head_ptr_);
            head_ptr_ = current;
            return true;
        }
    }
    return false;
}

/**
    * Merges the dishes and ingredients of two specified stations.
    * @param station_name1 The name of the first station.
    * @param station_name2 The name of the second station.
    * @post: The second station is removed from the list, and its
    contents are added to the first station.
    * @return: True if both stations were found and merged; false
    otherwise.
    */
bool StationManager::mergeStations(const std::string &station_name1, const std::string &station_name2) {
    KitchenStation* station1 =  findStation(station_name1);
    KitchenStation* station2 =  findStation(station_name2);

    if (station1 != nullptr && station2 != nullptr){
        // Move ingredients
        for (const Ingredient& ingredient: station2->getIngredientsStock()) {
            station1->replenishStationIngredients(ingredient);
        }

        //Move dishes
        for (Dish * dish: station2->getDishes()) {
            station1->assignDishToStation(dish);
        }

        station2->getDishes().clear();
        station2->getIngredientsStock().clear();

        for (int i = 0; i < item_count_; ++i) {
            Node<KitchenStation*> * current = getNodeAt(i);
            if (current->getItem()->getName() == station_name2){
                if (i == 0){
                    head_ptr_ = current->getNext();
                    delete current->getItem();
                    delete current;
                    item_count_=  item_count_ -1;
                    return true;

                }
                else{
                    Node<KitchenStation*> * prev = getNodeAt(i-1);
                    prev->setNext(current->getNext());
                    delete current->getItem();
                    delete current;
                    item_count_=  item_count_ -1;
                    return true;

                }

            }
        }
    }
    return false;
}

/**
    * Assigns a dish to a specific station.
    * @param station_name A string representing the station's name.
    * @param dish A pointer to a Dish object.
    * @post: Assigns the dish to the specified station.
    * @return: True if the station was found and the dish was assigned;
    false otherwise.
    */
bool StationManager::assignDishToStation(const std::string &station_name, Dish *dish) {
    KitchenStation* station = findStation(station_name);
    if (station != nullptr){
        return station->assignDishToStation(dish);
    }
    return false;
}

/**
    * Replenishes an ingredient at a specific station.
    * @param station_name A string representing the station's name.
    * @param ingredient An Ingredient object.
    * @post: Replenishes the ingredient stock at the specified station.
    * @return: True if the station was found and the ingredient was
    replenished; false otherwise.
    */
bool StationManager::replenishIngredientAtStation(const std::string &station_name, const Ingredient &ingredient) {
    KitchenStation* station = findStation(station_name);
    if (station != nullptr){
        station->replenishStationIngredients(ingredient);
        return true;
    }
    return false;
}

/**
    * Checks if any station in the station manager can complete an
    order for a specific dish.
    * @param dish_name A string representing the name of the dish.
    * @return: True if any station can complete the order; false
    otherwise.
    */
bool StationManager::canCompleteOrder(const std::string &dish_name) {

    for (int i = 0; i < item_count_; ++i) {
        KitchenStation* station = getEntry(i);
        if (station->canCompleteOrder(dish_name)){
            return true;
        }
    }
    return false;
}

/**
    * Prepares a dish at a specific station if possible.
    * @param station_name A string representing the station's name.
    * @param dish_name A string representing the name of the dish.
    * @post: If the dish can be prepared, reduces the quantities of the
    used ingredients at the station.
    * @return: True if the dish was prepared successfully; false
    otherwise.
    */
bool StationManager::prepareDishAtStation(const std::string &station_name, const std::string &dish_name) {
    KitchenStation * station = findStation(station_name);
    if (station != nullptr){
        return station->prepareDish(dish_name);
    }
    return false;
}



