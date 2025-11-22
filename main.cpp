#include "stdafx.h"
#include "RedBlackTree.h"
#include "BinaryTree.h"
using namespace std;


void structMenu(){
    cout << "1)Red Black Tree" << endl;
    cout << "2)Binary Tree " << endl;
    cout << "3)exit " << endl;
    cout << "Choose: ";
}

void mainRBTMenu(){
    cout << "1)Build tree from bracket notation" << endl;
    cout << "2)Create with multiply nums" << endl;
    cout << "3)Insert Node" << endl;
    cout << "4)Delete Node " << endl;
    cout << "5)Find Node" << endl;
    cout << "6)Pre-Order Search" << endl;
    cout << "7)In-Order Search" << endl;
    cout << "8)Post-Order Search" << endl;
    cout << "9)Level-Order Search" << endl;
    cout << "10)Print Tree" << endl;
    cout << "0)Back" << endl;
    cout << "Choose: ";
}

void mainSimpleBinaryMenu(){
    cout << "1)Create with multiply nums" << endl;
    cout << "2)Insert Node" << endl;
    cout << "3)Pre-Order Search" << endl;
    cout << "4)In-Order Search" << endl;
    cout << "5)Post-Order Search" << endl;
    cout << "6)Back" << endl;
    cout << "Choose: ";
}


int main(){
    RedBlackTree rbt;
    BinaryTree tree;
    int structure = 0, rbt_option = 0, simple_option = 0;
    do{
        structMenu();
        cin >> structure;
        switch(structure){
            case 1:{
                do {
                    mainRBTMenu();
                    cin >> rbt_option;
                    switch(rbt_option){
                        case 1:{
                            int dir;
                            cout << "1)Input" << endl;
                            cout << "2)From file" << endl;
                            cout << "Choose: ";
                            cin >> dir;
                            switch(dir){
                                case 1:{
                                    string expression;
                                    cout << "Input expression in bracket notation " << endl;
                                    cin.ignore();
                                    getline(cin, expression);
                                    rbt.parseAndBuild(expression);
                                    break;
                                }
                                case 2:{
                                    string fileName;
                                    cout << "Input file name" << endl;
                                    cin.ignore();
                                    getline(cin, fileName); 
                                    rbt.parseAndBuildFromFile(fileName);
                                    break;
                                }
                            }
                            break;
                        }
                        case 2:{
                            rbt.clear();
                            int size;
                            cout << "\nInput size: ";
                            while (!(cin >> size) || size <= 0) {
                                cout << "Error! Input a positive number: ";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            
                            cout << "Input " << size << " integers: ";
                            for (int i = 0; i < size; i++) {
                                int value;
                                while (!(cin >> value)) {
                                    cout << "Error! Input integer: ";
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                }
                                rbt.insert(value);
                            }
                            break;
                        }
                        case 3:{
                            int data;
                            cout << "Input number: ";
                            cin >> data;
                            rbt.insert(data);
                            break;
                        }
                        case 4:{
                            int data;
                            cout << "Input number: ";
                            cin >> data;
                            rbt.remove(data);
                            break;
                        }
                        case 5:{
                            int data;
                            cout << "Input data to find: ";
                            cin >> data;
                            Node* node = rbt.search(data);
                            if (node == nullptr){
                                cout << "Node doesnt exist" << endl;
                            }
                            else{
                                cout << "Node exist" << endl;
                            }
                            break;
                        }
                        case 6:
                            rbt.preOrderIterative();
                            cout << endl;
                            break;
                        case 7:
                            rbt.inOrderIterative();
                            cout << endl;
                            break;
                        case 8:
                            rbt.postOrderIterative();
                            cout << endl;
                            break;
                        case 9:
                            rbt.levelOrderIterative();
                            cout << endl;
                            break;
                        case 10:
                            rbt.printTree();
                            cout << endl;
                        case 0:
                            break;
                        default:
                            cout << "Wrong option" << endl;
                            break;
                    }
                }
                while (rbt_option != 0);
                break;
            }
            case 2:{
                do{
                    mainSimpleBinaryMenu();
                cin >> simple_option;
                switch(simple_option){
                    case 1:
                    {
                        tree.clear();
                        int size;
                        cout << "\nInput size: ";
                        while (!(cin >> size) || size <= 0) {
                            cout << "Error! Input a positive number: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                            
                        cout << "Input " << size << " integers: ";
                        for (int i = 0; i < size; i++) {
                            int value;
                            while (!(cin >> value)) {
                                cout << "Error! Input integer: ";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            tree.add(value);
                            }
                            break;
                    }
                    case 2:{
                        int data;
                        cout << "Input number: ";
                        cin >> data;
                        tree.add(data);
                        break;
                    }
                    case 3:
                        tree.dfs_preorder();
                        cout << endl;
                        break;
                    case 4:
                        tree.dfs_inorder();
                        cout << endl;
                        break;
                    case 5:
                        tree.dfs_postorder();
                        cout << endl;
                        break;
                    case 6:
                        break;
                    default:
                        cout << "Wrong option" << endl;
                        break;

                }
            }
            while(simple_option!=6);   
            }
            case 3:
                break;
            default:
                cout << "Wrong option" << endl;
                break;
        }
    }
    while(structure != 3);

}