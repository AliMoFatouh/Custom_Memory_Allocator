/*

    Simple Custom Memory Allocator
    made by Hanafy, Ali

    This main Function includes all the needed Functions and Variables

*/

///The Heap Block
/*


index:
0        1        2        3        4 ...
+--------+--------+--------+--------+----+
| size   | size   |        user data     |
|  MSB   |  LSB   |                      |
+--------+--------+----------------------+


*/


#include <iostream>
#include<string>
#include<ctype.h>
#include <climits>


#define HEAP_SIZE 64000

using namespace std;

void* heap_alloc(size_t size);

void heap_free(void* ptr);


//exists with main
int EnsureCorrectInput(string data);
///OUR HEAP
unsigned char heap[HEAP_SIZE] = {0};//MEMORY HEAP SEGMENT


int main(void){


    std::cout<<"===== HEAP ALLOCATOR TEST =====\n\n";


    // Allocate multiple blocks

    int* a = (int*)heap_alloc(sizeof(int));
    int* b = (int*)heap_alloc(sizeof(int));
    int* c = (int*)heap_alloc(sizeof(int));


    if(a == nullptr || b == nullptr || c == nullptr)
    {
        std::cout<<"Allocation Failed\n";
        return 1;
    }



while(1){//Ensure Correct Input
    string DataTestFormat;
    cout<<"write down the value of a: ";

    getline(cin, DataTestFormat);

    *a = EnsureCorrectInput(DataTestFormat);

    cout<<"write down the value of b: ";

    getline(cin, DataTestFormat);

    *b = EnsureCorrectInput(DataTestFormat);

    cout<<"write down the value of c: ";

    getline(cin, DataTestFormat);

    *c = EnsureCorrectInput(DataTestFormat);

    if(*a==INT_MIN || *b==INT_MIN || *c==INT_MIN){

        cout<<"Incorrect Input was Found, Try again"<<endl;
        continue;
    }

    break;
}



    std::cout<<"Values after allocation:\n";

    std::cout<<"A: "<<*a<<std::endl;
    std::cout<<"B: "<<*b<<std::endl;
    std::cout<<"C: "<<*c<<std::endl;



    std::cout<<"\nHeap first 30 bytes:\n";


    for(int i = 0; i < 30; i++)
    {
        std::cout<<(int)heap[i]<<" ";
    }



    // Free middle block

    std::cout<<"\n\nFreeing B...\n";

    heap_free(b);



    std::cout<<"\nHeap after freeing B:\n";


    for(int i = 0; i < 30; i++)
    {
        std::cout<<(int)heap[i]<<" ";
    }



    // Allocate again after free

    int* d = (int*)heap_alloc(sizeof(int));


    if(d == nullptr)
    {
        std::cout<<"\nAllocation D Failed\n";
        return 1;
    }


    *d = 400;


    std::cout<<"\n\nNew allocation D: "<<*d<<std::endl;



    std::cout<<"\nFinal Heap:\n";


    for(int i = 0; i < 30; i++)
    {
        std::cout<<(int)heap[i]<<" ";
    }



    std::cout<<"\n\n===== TEST FINISHED =====\n";


    return 0;

}

int EnsureCorrectInput(string data){

        int i = 0;
        if(data[0] == '-'){i++;}

        for(;i < data.length();i++){

             if(data[i] < '0' || data[i] > '9')
            {
                cout<<"Incorrect Input!!"<<endl;
                return INT_MIN;
            }

        }
        return stoi(data); ///Correct Input
}


///The Heap Block
/*


index:
0        1        2        3        4 ...
+--------+--------+--------+--------+----+
| size   | size   |        user data     |
|  MSB   |  LSB   |          Bytes       |
+--------+--------+----------------------+
                   ^
                   |
                   |
                Pointers


*/
void heap_free(void* ptr){


    char* pointer = (char*)ptr;

    // Move pointer back to META DATA
    pointer = pointer - 2;


    unsigned short int TotalNumberOfBytes =
        ((unsigned short int)pointer[0] << 8) | pointer[1]; // META DATA


    pointer[0] = 0x00;
    pointer[1] = 0x00;


    int counter = 0;

    //The location is now at user Data
    pointer = pointer + 2;


    while(counter < TotalNumberOfBytes){

        pointer[counter] = 0x00;
        counter++;

    }


    return;
}

void* heap_alloc(size_t size)
{
    /////////////////////////////////////////////
    // 1. Find an Empty-Large Enough Heap Block
    /////////////////////////////////////////////

    int i = 0;
    bool found = false;


    while(i < HEAP_SIZE)
    {
        int counter = 0;
        int j = i;

        // Check if this region has enough free bytes
        while(( counter < size + 2 ) &&  (j <= HEAP_SIZE-1) && ( heap[j] == 0x00 ) )
        {
            j++;
            counter++;
        }

        // Found enough space
        if(counter == size + 2)
        {
            found = true;
            break;

        }

        ///WILL ONLY STOP AT META DATA
        // Otherwise this block is occupied -- reading the NEXT META DATA
        unsigned short int READ_BLOCKS =
            ((unsigned short int)heap[i] << 8) | heap[i+1];     ///Size of the current Block (Data Bytes)


        // Skip metadata + data
        i = i + 2 + READ_BLOCKS;        /// i INCREMENTING Method {i + (Meta Data Bytes) + Data Bytes };
    }


    // No suitable block found
    if(!found)
    {
        return nullptr;
    }

    /////////////////////
    //2. Insert MetaData
    /////////////////////

    unsigned short int Meta_Data_NBytes = size;

    heap[i] = (Meta_Data_NBytes >> 8) & 0xFF; // most Significant Byte
    heap[i+1] = Meta_Data_NBytes & 0xFF;      // least Significant Byte


    /////////////////////
    //3. Return first Data address
    /////////////////////

    return &heap[i+2];
}
