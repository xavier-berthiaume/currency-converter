# Currency-Converter

Currency-converter is a lightweight app that provides insights on the current exchange rates for the worlds currencies and leading cryptocurrencies. Written in C++ using the Qt framework, the UI is responsive and fully supports English and French. Exchange API, written by Fawaz Ahmed powers the application and provides up to date data on currencies and exchange rates.

## Features

- **Browse the leading currencies and cryptocurrencies**: Exchange API provides up to date information on what currencies are on the market, as well as their exchange rate at the current moment.
- **Generate exchange tables**: The interface allows a user to easily look up the exchange rate between two specific currencies, or broadly look at a currency's exchange rate across the various currencies.

## Getting Started

### Prerequisites

To build and run Quick Notes, you need:

- **Qt Framework**: Version 5 or later (Qt 6 recommended for best performance).
- **CMake**: Version 3.5 or later.
- **C++ Compiler**: Supporting C++17 or later.
- **Network Access**: Offline functionalities are not supported.

## Installing using binaries

### Building the Application

1. Clone the repository:
   ```bash
   git clone https://github.com/xavier-berthiaume/currency-converter
   cd currency-converter 
   ```

2. Create a build directory and navigate to it:
   ```bash
   mkdir build
   cd build
   ```

3. Run CMake to configure the project:
   ```bash
   cmake ../currency-converter
   ```

4. Build the application:
   ```bash
   make
   ```

5. Run the application:
   ```bash
   ./currency-converter
   ```

## Usage

1. **Selecting a currency**:
    - Using the top-most combobox, select a currency to broadly inspect.
    - The value of the spinbox can then be modified to reflect a value to be 'exchanged' to other currencies.
    - The bottom-most combobox can then be used to compare a second currency against the first.

2. **Viewing the exchange table**:
    - Once a currency has been selected to inspect, an exchange table will be generated with the API's data.
    - The table contains data about every single currency that is fetched.

3. **Changing the language**:
    - The action menu at the top of the program will contain language settings under File > Settings > Language.
    - Currently English (CA) and French (CA) are the only supported languages.

## File Structure

- `main.cpp`: Entry point of the application.
- `mainwindow.*`: Implements the main user interface and application logic.
- `creditdialog.*`: A custom dialog widget crediting me and the API .
- `createnotedialog.*`: A custom dialog widget used to create and edit notes.
- `currencyapimanager.*`: The network manager that fetches and updates data related to the Exchange API.
- `settingsdialog.*`: The dialog widget used to change parameters such as the software's current language.
- `conversiontable.*`: A custom widget that displays the relative exchange rates for a specific currency.

## Contributions

Contributions are welcome! If you have ideas for new features, feel free to open an issue or submit a pull request.

## License

This project is licensed under the GPL (GNU General Public License). See the `LICENSE` file for details.

---

Enjoy using Currency-Converter to stay up to date on the current exchange rates for the most relevant currencies on the market!

