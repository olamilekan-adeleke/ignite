//
// #include <iostream>
// #include <string>
//
// struct ProductConfig {
//   std::string name;
//   double price = 0.0;            // Default value
//   int quantity = 1;              // Default value
//   std::string currency = "USD";  // Default value
// };
//
// // Class that uses ProductConfig
// class Product {
//  public:
//   Product(ProductConfig config) : config_(config) {}
//
//   void display() const {
//     std::cout << "Product: " << config_.name << ", Price: " << config_.price << " " << config_.currency
//               << ", Quantity: " << config_.quantity << std::endl;
//   }
//
//  private:
//   ProductConfig config_;
// };
//
// int main() {
//   // Pass only name and quantity, order doesn't matter
//   Product item1({.name = "Laptop", .quantity = 2});
//   item1.display();  // Output: Product: Laptop, Price: 0 USD, Quantity: 2
//
//   // Pass name and price, using default for quantity and currency
//   Product item2({.name = "Mouse", .price = 25.99});
//   item2.display();  // Output: Product: Mouse, Price: 25.99 USD, Quantity: 1
//
//   // Pass all arguments, order still doesn't matter
//   Product item3({.quantity = 5, .currency = "EUR", .name = "Keyboard", .price = 75.0});
//   item3.display();  // Output: Product: Keyboard, Price: 75 EUR, Quantity: 5
//
//   return 0;
// }
