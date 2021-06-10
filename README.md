# Project name: Words prediction system based on N-gram Language Model

#### Semester project of Architecture of Computer System course

## Project description 
Our project's aim is to implement autosuggestion system based on n-gram language model.
Firstly, the user uploads the text corpus to train the probabilistic model.
Next, after n-grams are extracted and their probabilities are calculated,
the user starts inputing text and our interface suggests possible endings of the phrase.

Moreover, there are statistical data on the corpus uploaded, such as the most popular n-grams.

We have implemented three algorithms: the simple one, which works with strings (words) themselves,
the one that works with string's hashes and the one that uses database. For our final release, we
chose the best one.

## Requirements

To run the program:
* QT5
* CMake 3.17
* C++ standard 17
* Boost
* OpenMP

## Usage

1. Clone the repository and cd into its folder.
```bash
git clone https://github.com/romanyshyn-natalia/Word_prediction_system_based_on_n-Gram.git
cd Word_prediction_system_based_on_n-Gram
```

2. To run the application, build the project.
```bash
mkdir build; cd build
cmake ..
cmake --build .
```

3. Run the application.
```bash
./n_gram  # you should stay in build directory
```

4. Have fun! ```>^-^<```

## Contributors
* [Romanyshyn Natalia](https://github.com/romanyshyn-natalia)
* [Korabliova Anna](https://github.com/anika02)
* [Hromyak Diana](https://github.com/Diana-Doe)
* [Myntiuk Sofiia](https://github.com/sophmintaii)