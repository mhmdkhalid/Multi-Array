#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

template <typename T>
class MultiArray {
private:
    int numOfArrs;
    int* sizes;
    string* names;
    T** arrays;

public:

    MultiArray(int numofarrs, int* Sizes, string* Names) {
        numOfArrs = numofarrs;
        sizes = new int[numofarrs];
        names = new string[numofarrs];
        arrays = new T*[numofarrs];

        for (int i = 0; i < numofarrs; i++) {
            sizes[i] = Sizes[i];
            names[i] = Names[i];
            arrays[i] = new T[sizes[i]]{};
        }
    }


    ~MultiArray() {
        for (int i = 0; i < numOfArrs; i++) {
            delete[] arrays[i];
        }
        delete[] arrays;
        delete[] names;
        delete[] sizes;
    }


    MultiArray(const MultiArray& obj) {
        numOfArrs = obj.numOfArrs;
        sizes = new int[numOfArrs];
        names = new string[numOfArrs];
        arrays = new T*[numOfArrs];

        for (int i = 0; i < numOfArrs; i++) {
            sizes[i] = obj.sizes[i];
            names[i] = obj.names[i];
            arrays[i] = new T[sizes[i]];

            for (int j = 0; j < sizes[i]; j++) {
                arrays[i][j] = obj.arrays[i][j];
            }
        }
    }

    void loadFromFile(string arrName, string fileName) {
        ifstream infile(fileName);
        if (!infile.is_open()) {
            cout << "Error opening file" << endl;
            return;
        }

        for (int i = 0; i < numOfArrs; i++) {
            if (arrName == names[i]) {
                int j = 0;
                while (j < sizes[i] && infile >> arrays[i][j]) {
                    cout << arrays[i][j] << endl;
                    j++;
                }
            }
        }

        infile.close();
    }

    T* subArray(string name, int startIndex, int endIndex, int step = 1) {
        for (int i = 0; i < numOfArrs; i++) {
            if (names[i] == name) {
                int subArraySize = ((endIndex - startIndex) / step) + 1;
                T* sub = new T[subArraySize];
                int subIndex = 0;

                if (startIndex <= endIndex) {
                    for (int j = startIndex; j <= endIndex; j += step) {
                        sub[subIndex++] = arrays[i][j];
                    }
                } else {
                    for (int j = startIndex; j >= endIndex; j += step) {
                        sub[subIndex++] = arrays[i][j];
                    }
                }
                return sub;
            }
        }
        cout << "Array not found: " << name << endl;
        return nullptr;
    }

    void reverse(string name) {
        cout<<"array after reversing: ";
        for (int i = 0; i < numOfArrs; i++) {
            if (names[i] == name) {
                int start = 0;
                int end = sizes[i] - 1;

                while (start < end) {
                    T temp = arrays[i][start];
                    arrays[i][start] = arrays[i][end];
                    arrays[i][end] = temp;
                    start++;
                    end--;
                }
                for (int j =0 ;j<sizes[i];j++){
                    cout<<arrays[i][j]<<" ";
                }
                return;
            }
        }
        cout << "Array not found: " << name << endl;
    }


    void sort(string name) {
        cout<<"\nArray after sorting: "<<endl;
        for (int i = 0; i < numOfArrs; ++i) {
            if (names[i] == name) {
                std::sort(arrays[i], arrays[i] + sizes[i]);
                for (int j =0 ;j<sizes[i];j++){
                    cout<<arrays[i][j]<<" ";
                }
                return;
            }
        }
        cout << "Array not found: " << name << endl;
    }

    bool search(T item) {
        for (int i = 0; i < numOfArrs; i++) {
            for (int j = 0; j < sizes[i]; j++) {
                if (arrays[i][j] == item) {
                    return true;
                }
            }
        }
        return false;
    }


    T& operator()(string name, int index) {
        for (int i = 0; i < numOfArrs; i++) {
            if (names[i] == name) {
                if (index >= 0) {
                    return arrays[i][index];
                } else {

                    int negIndex = sizes[i] + index;
                    return arrays[i][negIndex];
                }
            }
        }
    }


    MultiArray split(string name) {
        MultiArray<T> splitArrays(2, new int[2]{sizes[0] / 2, sizes[0] - sizes[0] / 2}, new string[2]{"first", "second"});
        for (int i = 0; i < numOfArrs; i++) {
            if (names[i] == name) {
                int firstSize = sizes[i] / 2;
                int secondSize = sizes[i] - firstSize;

                for (int j = 0; j < firstSize; j++) {
                    splitArrays.arrays[0][j] = arrays[i][j];
                }

                for (int j = 0; j < secondSize; j++) {
                    splitArrays.arrays[1][j] = arrays[i][firstSize + j];
                }

                return splitArrays;
            }
        }
        cout << "Array not found: " << name << endl;
        return splitArrays;
    }


    T* merge() {
        int totalSize = 0;
        for (int i = 0; i < numOfArrs; i++) {
            totalSize += sizes[i];
        }

        T* mergedArray = new T[totalSize];
        int currentIndex = 0;

        for (int i = 0; i < numOfArrs; i++) {
            for (int j = 0; j < sizes[i]; j++) {
                mergedArray[currentIndex++] = arrays[i][j];
            }
        }

        return mergedArray;
    }


    template <typename U>
    void applyToArray(string name, void (*func)(U&)) {
        for (int i = 0; i < numOfArrs; i++) {
            if (names[i] == name) {
                for (int j = 0; j < sizes[i]; j++) {
                    func(arrays[i][j]);
                }
                return;
            }
        }
        cout << "Array not found: " << name << endl;
    }


    template <typename T2>
    void square(T2& x) {
        x = x * x;
    }


    friend ostream& operator<<(ostream& os, const MultiArray<T>& arr) {
        for (int i = 0; i < arr.numOfArrs; i++) {
            os << "Array Name: " << arr.names[i] << endl;
            for (int j = 0; j < arr.sizes[i]; j++) {
                os << arr.arrays[i][j] << " ";
            }
            os << endl;
        }
        return os;
    }

    template<typename U>
    void createAndFillFile(const string& fileName) {
        ofstream outfile(fileName);
        if (!outfile.is_open()) {
            cout << "Error opening file" << endl;
            return;
        }

        U data;
        cout << "Enter data for the file AND do not exceed the size (Ctrl+Z/Ctrl+D on a single line to stop):" << endl;
        while (cin >> data) {
            outfile << data <<endl;
        }

        outfile.close();
    }

    void fillArraysManually() {
        for (int i = 0; i < numOfArrs; ++i) {
            cout << "Enter data for Array " << names[i] << ":" << endl;
            for (int j = 0; j < sizes[i]; ++j) {
                cout << "Enter value at index " << j << " for Array " << names[i] << ": ";
                cin >> arrays[i][j];
            }
        }
    }

};

template <typename T2>
void square(T2& x) {
    x = x * x;
}


template<typename T>
void performOperations() {
    int numOfArrays;
    cout << "Enter the number of arrays: ";
    cin >> numOfArrays;

    int* sizes = new int[numOfArrays];
    string* names = new string[numOfArrays];

    for (int i = 0; i < numOfArrays; ++i) {
        cout << "Enter size for Array " << i + 1 << ": ";
        cin >> sizes[i];
        cout << "Enter name for Array " << i + 1 << ": ";
        cin >> names[i];
    }

    MultiArray<T> myMultiArray(numOfArrays, sizes, names);
    myMultiArray.fillArraysManually();

    for (int j = 0; j < numOfArrays; ++j) {
        cout << "Available Arrays:" << endl;
        for (int i = 0; i < numOfArrays; ++i) {
            cout << i + 1 << ". " << names[i] << endl;
        }

    }
    string chosenArrayToLoad;
    cout << "Enter the array name you want to load to: ";
    cin >> chosenArrayToLoad;

    string fileName;
    cout << "Enter the file name to create and load from (add .txt at the end): ";
    cin >> fileName;

    myMultiArray.template createAndFillFile<T>(fileName);
    cout << "Values loaded from file for " << chosenArrayToLoad << ": " << endl;
    myMultiArray.loadFromFile(chosenArrayToLoad, fileName);


    cout << "Available Arrays:" << endl;
    for (int i = 0; i < numOfArrays; ++i) {
        cout << i + 1 << ". " << names[i] << endl;
    }
    cout << "Enter the array name you want to edit: ";
    string chosenArray;
    cin >> chosenArray;

    cout<<"Enter indices to create a sub array: ";
    int start,end;
    cin>>start>>end;
    if (start >= 0 && end < sizes[0] && start <= end) {
        T* subArr = myMultiArray.subArray(chosenArray, start, end);
        if (subArr != nullptr) {
            cout << "Subarray: ";
            for (int i = 0; i < (end - start + 1); ++i) {
                cout << subArr[i] << " ";
            }
            cout << endl;
            delete[] subArr;
        } else {
            cout << "Failed to create sub-array." << endl;
        }
    } else {
        cout << "Invalid indices entered for sub-array." << endl;
    }

    myMultiArray.reverse(chosenArray);

    myMultiArray.sort(chosenArray);

    cout<<"Enter item to search for in the array: ";
    T item;
    cin>>item;
    bool found = myMultiArray.search(item);
    cout << "\nFound 7: " << boolalpha << found << endl;

    int indexSearch;
    cout<<"Enter the index of value u are looking for: ";
    cin>>indexSearch;
    T& value = myMultiArray(chosenArray, indexSearch);
    cout << "Value at index 2: " << value << endl;


    MultiArray<T> splitArray = myMultiArray.split(chosenArray);
    cout << "Split Arrays: ";
    cout << splitArray << endl;


    T* mergedArray = myMultiArray.merge();
    cout << "Merged Array: ";
    for (int i = 0; i < 12; ++i) {
        cout << mergedArray[i] << " ";
    }
    cout << endl;
    delete[] mergedArray;



    delete[] sizes;
    delete[] names;
}


int main(){

    cout << "Enter the data type for arrays (int, double, string, etc.): ";
    string dataType;
    cin >> dataType;
    if (dataType == "int") {
        performOperations<int>();
    } else if (dataType == "double") {
        performOperations<double>();
    } else if (dataType == "string") {
        performOperations<string>();
    }


    return 0;

}