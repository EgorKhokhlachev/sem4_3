#include <iostream>
#include <fstream>
#include <sstream>
//#include <string>
#include <algorithm> 
#include <climits>  
#include <cstring>  

class BoundedPriorityQueue {
protected: // Make data and size_ accessible to derived classes
    int* data;
    size_t capacity;
    size_t size_;

    // Helper function to heapify a subtree rooted at index 'i'
    void heapify(size_t i) {
        size_t largest = i;
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < size_ && data[left] > data[largest]) {
            largest = left;
        }

        if (right < size_ && data[right] > data[largest]) {
            largest = right;
        }

        if (largest != i) {
            std::swap(data[i], data[largest]);
            heapify(largest);
        }
    }

public:
    // Constructor
    BoundedPriorityQueue(size_t capacity_) : data(new int[capacity_]), capacity(capacity_), size_(0) {}

    // Copy Constructor
    BoundedPriorityQueue(const BoundedPriorityQueue& other) : data(new int[other.capacity]), capacity(other.capacity), size_(other.size_) {
        std::memcpy(data, other.data, sizeof(int) * size_);
    }

    // Move Constructor
    BoundedPriorityQueue(BoundedPriorityQueue&& other) noexcept : data(other.data), capacity(other.capacity), size_(other.size_) {
        other.data = nullptr;
        other.capacity = 0;
        other.size_ = 0;
    }

    // Copy Assignment Operator
    BoundedPriorityQueue& operator=(const BoundedPriorityQueue& other) {
        if (this != &other) {
            delete[] data;
            data = new int[other.capacity];
            capacity = other.capacity;
            size_ = other.size_;
            std::memcpy(data, other.data, sizeof(int) * size_);
        }
        return *this;
    }

    // Move Assignment Operator
    BoundedPriorityQueue& operator=(BoundedPriorityQueue&& other) noexcept {
        if (this != &other) {
            delete[] data;

            data = other.data;
            capacity = other.capacity;
            size_ = other.size_;

            other.data = nullptr;
            other.capacity = 0;
            other.size_ = 0;
        }
        return *this;
    }

    // Function to push an element into the priority queue
    void push(int value) {
        if (size_ < capacity) { // Ensure we don't exceed MAX_SIZE
            data[size_] = value;
            int i = size_;
            ++size_;
            while (i != 0 && data[(i - 1) / 2] < data[i]) {
                std::swap(data[i], data[(i - 1) / 2]);
                i = (i - 1) / 2;
            }
        } else {
            std::cerr << "Warning: Priority queue is full. Cannot push element " << value << std::endl;
        }
    }

    // Function to pop (remove) the element with the highest priority (largest value)
    void pop() {
        if (!empty()) {
            data[0] = data[size_ - 1];
            --size_;
            heapify(0);
        }
    }

    // Function to return the element with the highest priority (largest value)
    int top() const {
        if (!empty()) {
            return data[0];
        } else {
            std::cerr << "Error: Priority queue is empty. Returning INT_MIN.\n";
            return INT_MIN; 
        }
    }

    // Function to check if the priority queue is empty
    bool empty() const {
        return size_ == 0;
    }

    // Function to get the size of the priority queue
    size_t size() const {
        return size_;
    }

    // Pre-decrement operator (--pq)
    BoundedPriorityQueue& operator--() {
        pop(); 
        return *this;
    }

    // Post-decrement operator (pq--)
    BoundedPriorityQueue operator--(int) {
        BoundedPriorityQueue temp = *this;  // Create a copy
        pop();        // Modify the original
        return temp;      // Return the copy
    }

    // Overload + to add a number to the queue (acts as push)
    BoundedPriorityQueue& operator+(int value) {
        push(value);
        return *this;
    }

    // Overload << operator for output stream
    friend std::ostream& operator<<(std::ostream& os, const BoundedPriorityQueue& pq) {
        os << "Queue: ";
        for (size_t i = 0; i < pq.size(); ++i) {
            os << pq.data[i] << " ";
        }
        return os;
    }

    virtual ~BoundedPriorityQueue() {
        delete[] data;
    }
};

class Base : public BoundedPriorityQueue {
protected:
    char* filename;
    public:
    //Constructor
    Base(size_t capacity, const char* filename_) : BoundedPriorityQueue(capacity) {
        filename = new char[std::strlen(filename_) + 1];
        std::strcpy(filename, filename_);
    }

    //Copy Constructor
    Base(const Base& other) : BoundedPriorityQueue(other) {
        filename = new char[std::strlen(other.filename) + 1];
        std::strcpy(filename, other.filename);
    }

    //Move Constructor
    Base(Base&& other) noexcept : BoundedPriorityQueue(std::move(other)), filename(other.filename) {
        other.filename = nullptr;
    }

    //Copy assignmnet Operator
    Base& operator=(const Base& other) {
        if (this != &other) {
            BoundedPriorityQueue::operator=(other);
            delete[] filename;
            filename = new char[std::strlen(other.filename) + 1];
            std::strcpy(filename, other.filename);
        }
        return *this;
    }

    //Move Assignmnet operator
    Base& operator=(Base&& other) noexcept {
        if (this != &other) {
            BoundedPriorityQueue::operator=(std::move(other));
            delete[] filename;
            filename = other.filename;
            other.filename = nullptr;
        }
        return *this;
    }

    char* getFilename() const { return filename; }

    virtual int output() = 0; // Pure virtual function.

    virtual ~Base() {
        delete[] filename;
    }
};

class Child0 : public Base {
public:
    //Constructor
    Child0(size_t capacity,const char* filename) : Base(capacity,filename) {}

    //Copy Constructor
    Child0(const Child0& other) : Base(other) {}

    //Move Constructor
    Child0(Child0&& other) noexcept : Base(std::move(other)) {}

    //Copy assignmnet Operator
    Child0& operator=(const Child0& other) {
        if (this != &other) {
            Base::operator=(other);
        }
        return *this;
    }

    //Move Assignmnet operator
    Child0& operator=(Child0&& other) noexcept {
        Base::operator=(std::move(other));
        return *this;
    }

    int output() override {
        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for output." << std::endl;
            return -1;
        }

        for (size_t i = 0; i < size(); ++i) {
            outputFile << data[i] << (i == size() - 1 ? "" : " ");
        }
        outputFile << std::endl;
        outputFile.close();
        return 0;
    }

    ~Child0() {}
};

class Child1 : public Base {
public:

    //Constructor
    Child1(size_t capacity,const char* filename) : Base(capacity,filename) {}

    //Copy Constructor
    Child1(const Child1& other) : Base(other) {}

    //Move Constructor
    Child1(Child1&& other) noexcept : Base(std::move(other)) {}

    //Copy assignmnet Operator
    Child1& operator=(const Child1& other) {
        if (this != &other) {
            Base::operator=(other);
        }
        return *this;
    }

    //Move Assignmnet operator
    Child1& operator=(Child1&& other) noexcept {
        Base::operator=(std::move(other));
        return *this;
    }

    int output() override {
        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Could not open file " << filename << " for output." << std::endl;
            return -1;
        }

        for (size_t i = 0; i < size(); ++i) {
            outputFile << data[i] << std::endl;
        }
        outputFile.close();
        return 0;
    }

    ~Child1() {}
};

void loadFromString(BoundedPriorityQueue* queue, char* dataString) {
    char* token = std::strtok(dataString, " "); // Tokenize the string by spaces
    while (token != nullptr) {
        try {
            int value = std::stoi(token); // Convert token to integer
            queue->push(value);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Invalid input " << token << std::endl;
        }
        token = std::strtok(nullptr, " "); // Get the next token
    }
}

Base* Create_Data(const char* str){
    int type;
    char* filename = new char[strlen(str)];
    int capacity;
    char* data = new char[strlen(str)];

    // Parse the line using sscanf
    if (sscanf(str, "%d %s %[^\n]", &type, filename, data) != 3){
        std::cerr << "Error: Invalid format in input file." << std::endl;
        return nullptr;
    } 
    Base* queue = nullptr;
    if (type == 0) {
        queue = new Child0(strlen(data),filename);
        std::cout << "capacity of data0 - " << strlen(data) << std::endl;
    } else if (type == 1) {
        queue = new Child1(strlen(data),filename);
        std::cout << "capacity of data1 - " << strlen(data) << std::endl;
} if(type != 0 && type != 1) {
        std::cerr << "Error: Invalid type in input file." << std::endl;
        return nullptr;
    }

    loadFromString(queue, data);

    delete[] filename;
    delete[] data;

    return queue;
}

char *mygets(char *&str, size_t &LMax, FILE *in){
    int begin=0;
    if(!str)
            str=new char[LMax];
    while(fgets(str+begin,LMax-begin,in)){
            begin=LMax-1; 
            if(strchr(str,'\n')) 
                    break;
            LMax*=2;
            char *buf=new char[LMax];
            strcpy(buf, str);
            delete [] str;
    str=buf;
    }
    if(begin==0){
            delete [] str;
            return nullptr;
    }
    return str;
}

Base** resize(Base** arr, int* MAX_OBJS){
    *MAX_OBJS = (*MAX_OBJS)*2;
    Base** arr1 = new Base*[*MAX_OBJS];
    for(int i = 0; i< *MAX_OBJS;i++){
        arr1[i] = arr[i];
    }
    delete[] arr;
    return arr1;
}

int main() {
    int MAX_OBJS = 2;
    Base** arr = new Base*[MAX_OBJS];
    int queueCount = 0;
    size_t LMAX = 10;


    FILE* inputFile = fopen("input.txt","r");
    if (inputFile == NULL){
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    /*std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }*/

    char* line = nullptr; // Buffer for reading each line

    while (mygets(line,LMAX,inputFile) != nullptr) {
        std::cout << "in while" << std::endl;
        if (queueCount < MAX_OBJS){arr = resize(arr,&MAX_OBJS);}
        arr[queueCount] = Create_Data(line);
        //delete[] line;
        if (arr[queueCount] != nullptr) {
            queueCount++;
        }
    }

    for (int i = 0; i < queueCount; ++i) {
        arr[i]->output();
        std::cout << "Data written to - " << arr[i]->getFilename() << std::endl;
        std::cout << *arr[i] << std::endl;
    }

    Base* temp = arr[0];
    for (int i = 0; i < queueCount - 1; ++i) {
        arr[i] = arr[i + 1];
    }
    arr[queueCount - 1] = temp;

    std::cout << "Printing after cyclic shift\n";

        // Output the data for each object
    for (int i = 0; i < queueCount; ++i) {
        arr[i]->output();
        std::cout << "Data written to - " << arr[i]->getFilename() << std::endl;
        std::cout << *arr[i] << std::endl;
    }

    BoundedPriorityQueue test = *arr[0];
    std::cout << test << std::endl;

    test----;
    std::cout << test << std::endl;

    test + 99;
    std::cout << test << std::endl;

    fclose(inputFile);
    for (int i = 0; i < queueCount; ++i) {
        delete arr[i];
    }
    delete[] arr;
    return 0;
}