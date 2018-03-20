#include<iostream>
#include<vector>
#include <algorithm>

using namespace std;


template<class T>
class Node
{
public:
	int i;// row id
	int j;// col id 
	T v;  // element value
	Node* right;
	Node* down;
	// construction for struct node
	Node(int i, int j, T v)
	{
		this->i = i;
		this->j = j;
		this->v = v;
		this->right = nullptr;
		this->down = nullptr;
	}
	Node()
	{
		this->i = 0;
		this->j = 0;
		this->v = 0;
		this->right = nullptr;
		this->down = nullptr;
	}
};




template<class T>
class Sparsematrix
{
public:
	int MAXNonzero;// the maximum of nonzero element
	int row;// row of matrix
	int col;// col of matrix
	int nonzero;// the num or counter of nonzero elememts
	Node<T>* data;

	/***
	* construction function
	* para:
	* MAXNonzero: nonzero element in matrix
	* row: row of matrix
	* col: col of matrix
	*/
	Sparsematrix(int maxnum, int md, int nd) :MAXNonzero(maxnum), row(md), col(nd), nonzero(0)
	{
		data = new Node<T>[row];
	}

	Sparsematrix(int md, int nd) :row(md), col(nd), nonzero(0)
	{
		MAXNonzero = row*col;
		data = new Node<T>[row];
	}

	/*********
	* get a row of matrix by id
	*/
	vector<T> getRow(int i)
	{
		vector<T> result;
		Node<T>* p = &data[i];
		cout << "Row" << i << ": ";

		int lastj = 0;
		while (p)
		{
			while (lastj < p->j)
			{
				result.push_back(0);
				lastj++;
			}
			result.push_back(p->v);
			p = p->right;
			lastj++;
		}
		// suppose the row ends up with 0 strings
		while (lastj < this->col)
		{
			result.push_back(0);
			lastj++;
		}
		return result;
	}

	// get a col by id
	vector<T> getCol(int colid)
	{
		vector<T> result;
		for (int i = 0; i < this->row; i++)
		{
			result.push_back(at(i, colid));
		}
		return result;
	}

	// print a col by id
	void printCol(int colid) {
		if (colid > this->col) {
			cout << "Col out of range" << endl;
			return;
		}
		vector<T> s = getCol(colid);
		cout << "Col " << colid << ":";
		for (int i = 0; i < s.size(); i++)
		{
			cout << s[i] << "  ";
		}
		cout << endl;
	}

	// print a row by id
	void printRow(int rowid) {
		if (rowid > this->row) {
			cout << "Row out of range" << endl;
			return;
		}
		vector<T> s = getRow(rowid);
		cout << "Row " << rowid << ":";
		for (int i = 0; i < s.size(); i++)
		{
			cout << s[i] << "  ";
		}
		cout << endl;
	}

	void printmatrix() {
		cout << "matrix " << this->row << "*" << this->col << ":" << endl;
		for (int i = 0; i < this->row; i++) {
			/*this->printRow(i);*/
			for (int j = 0; j < this->col; j++) {
				cout << this->at(i, j) << "  ";
			}
			cout << endl;
		}
	}

	/**********
	* Initialize the matrix
	*/
	void Initialize(int md, int nd)
	{
		delete this->data;
		this->nonzero = 0;
		this->row = md;
		this->col = nd;
		MAXNonzero = row*col;
		data = new Node<T>[md];;
		Node<T>* p = data;
	}

	/*****
	* get the pointer of element by coordinate x,y
	*/
	T at(int i, int j)
	{
		if (i < 0 || i >= this->row || j < 0 || j >= this->col) {
			cout << "out of range" << endl;
			return 0;
		}
		Node<T>* line = &data[i];
		Node<T>* ele = getElementofLine(line,j);
		if (ele == nullptr) {
			return 0;
		}
		return ele->v;
	}

	/**********
	* insert a nonzero value into matrix
	*/
	Node<T>* insert(T val, int i, int j) {
		// warning for saturation or wrong index i,j
		if (nonzero == MAXNonzero || i > row || j > col)
		{
			cout << "The matrix is saturation, fail to insert (" << i << "," << j << ")" << endl;
			return nullptr;
		}

		if (val == 0) {
			//cout << "There is no point to insert a zero." << endl;
			return nullptr;
		}

		Node<T>* p = &data[i]; // find the head of row
		while (p) {
			if (p->j == j) {
				if (p->v == 0 && val != 0)
				{
					nonzero++; // counter for nonzero
				}
				p->v = val; // assign the value to element of matrix
				return p;
			}

			// insert between the nodes
			if(p->j < j && p->right && p->right->j > j )
			{
				Node<T>* newele= new Node<T>(i,j,val);
				nonzero++;
				newele->right = p->right;
				p->right = newele;
				return newele;
			}

			// insert at the end
			if ( p->right == nullptr)
			{
				Node<T>* newele = new Node<T>(i, j, val);
				nonzero++;
				p->right = newele;
				return newele;
			}
			p = p->right;
		}

	}

	/********
	* Initialize the matrix with 3 vectors
	*/
	void initializeFromVector(vector<int> rows, vector<int> cols, vector<T> vals)
	{
		auto maxrow = max_element(rows.begin(), rows.end());
		auto maxcol = max_element(cols.begin(), cols.end());
		int size = *maxrow;
		int newrow = *maxrow + 1; // get the maximum row element from the vector
		int newcol = *maxcol + 1; // get the maximum col element from the vector
		Initialize(newrow, newcol); // initialize the matrix
		for (int i = 0; i < rows.size(); i++) {
			int ti = rows[i];
			int tj = cols[i];
			T value = vals[i];
			insert(value,ti,tj);
		}
	}

	/********
	* get the node from the line according to j coordinate
	*/
	Node<T>* getElementofLine(Node<T>* line, int j)
	{
		Node<T>* p = line;
		while (p)
		{
			if (p->j == j) {
				return p;
			}
			p = p->right;
		}
		return nullptr;
	}

	/*******************
	*  Solving Linear Equation with Gauss_Seidel theory 
	*/
	void  Gauss_Seidel(double B[],double X[])
	{
		double **A = new double*[row];
		for (int i = 0; i < row; i++)
			A[i] = new double[col];
		int n = this->col;

		for (int i = 0; i < this->row; i++) {
			for (int j = 0; j < this->col; j++) {
				A[i][j] = at(i, j);
			}
		}
		for (int k = 0; k < 1000; k++)
		{
			for (int i = 0; i < n; i++)
			{
				double sum = 0;
				for (int j = 0; j < n; j++)
				{
					if (j == i) continue; //跳过aii  
					sum += A[i][j] * X[j];
				}
				X[i] = (B[i] - sum) / A[i][i];  ///计算完新的x[i],旧的x[i]会被自然冲掉  
			}
		}
	}
};


int main() {
	Sparsematrix<int> s(3, 2 ,2);
	s.insert(5, 0, 0);
	s.insert(1, 0, 1);
	s.insert(2, 1, 0);
	s.insert(3, 1, 1);
	cout << s.at(0, 0) << endl;
	cout << s.at(0, 1) << endl;
	cout << s.at(1, 0) << endl;
	cout << s.at(1, 1) << endl;
	cout << "---------------------------" << endl;
	vector<int> a = { 0,1,2 };
	vector<int> b = { 0,1,2 };
	vector<int> c = { 7,8,9 };
	s.initializeFromVector(a, b, c);
	s.printmatrix();
	cout << "---------------------------" << endl;
	vector<int> r1 = { 0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3 };
	vector<int> c1 = { 0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3 };
	vector<int> v1 = { 10,-1,2,0,-1,11,-1,3,2,-1,10,-1,0,3,-1,8 };
	s.initializeFromVector(r1, c1, v1);
	s.printmatrix();
	cout << "---------------------------" << endl;
	double X[] = { 0,0,0,0 };
	double B[] = { 6,25,-11,15 };
	s.Gauss_Seidel(B,X);
	for (int i = 0; i < s.col; i++)
		cout << X[i] << '\t';
	cout << endl;
	getchar();
}