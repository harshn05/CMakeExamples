//g++ -fopenmp -static -O3 -o grow2d.exe grow2d.cpp -lgomp -ldl
//cl.exe /EHsc /openmp /Ox grow2d.cpp
//clang & gcc clang++ -target x86_64-pc-windows-gnu -fopenmp -O3 -o grow2d.exe grow2d.cpp

#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<vector>
#include<fstream>


using namespace std;
int m, n, p;

bool iscomplete(const vector<int>& I)
{

	for (int i = 0; i < I.size(); ++i)
	{
		if (I[i] == 0)
		{

			return false;
		}
	}
	return true;
}


int main(int argc, char *argv[])
{

	srand(time(NULL));
	
	if(argc==1)
	{
		m = 1000;
		n = 1000;
		p = 40;
	}	
	else
	{
		m = atoi(argv[1]);
		n = atoi(argv[2]);
		p = atoi(argv[3]);
	}

	double t1 = clock();

	int xo;
	int yo;
	const  int s = m*n;
	int count = 0;
	vector<int> I(s,0);
	std::vector <double> X,Y,Z,a;
    X.resize(p);
	Y.resize(p);
	Z.resize(p);
	a.resize(p);

	int N,E,W,S,val1,val2,val3,val4;
	N=0;E=0;W=0;S=0;
	int low;
	for (int i = 0; i < s; i++)
	{
		I[i] = 0;
	}

	int index;


	for (int i = 0; i < p; i++)
	{
		a[i] = 1;

	}
	

	int label = 1;
	int px;
	int py;
	while (label < p)
	{
		xo = (m - 2)* double(rand()) / RAND_MAX + 1;
		yo = (n - 2)* double(rand()) / RAND_MAX + 1;
		index = (n)*xo + yo;
		if (I[index] == 0)
		{
			I[index]= label;
			X[label - 1] = xo;
			Y[label - 1] = yo;
			label++;
		}
	}

	bool complete = false;
	int r = 2;

	while (!complete)
	{
		#pragma omp parallel for private(xo,yo,label,low,px,py,N,E,W,S,index,val1, val2,val3,val4) schedule(dynamic)
		for (int k = 0; k < p; ++k)
		{

			if (a[k] == 1)
			{

				xo = X[k];
				yo = Y[k];
				a[k] = 0;

				label = k + 1;
				low = 0;
				for (int i = 0; i <= r; ++i)
				{
					for (int j = 0; j <= r; ++j)
					{

						if (i*i + j*j < r*r)
						{
							px = xo + i;
							py = yo + j;

							if (px >= 0 && px < m && py >= 0 && py < n)
							{
								index = (n)*px + py;

								if (I[index]  == 0)
								{

									N = index - n ;
									E = index + n ;
									W = index + 1 ;
									S = index - 1 ;

									if(N>=0 && N<s)
									{
										val1 = I[N];
									}
									else
									{
										val1 = 0;
									}

									if(E>=0 && E<s)
									{
										val2 = I[E];
									}
									else
									{
										val2 = 0;
									}

									if(W>=0 && W<s)
									{
										val3 = I[W];
									}
									else
									{
										val3 = 0;
									}

									if(S>=0 && S<s)
									{
										val4 = I[S];
									}
									else
									{
										val4 = 0;
									}


									if(val1==label||val2==label||val3==label||val4==label)
									{
										I[index]  = label;
										a[k] = 1;
									}
									


								}
							}

							px = xo - i;
							py = yo + j;

							if (px >= 0 && px < m && py >= 0 && py < n)
							{
								index = (n)*px + py;

								if (I[index]  == 0)
								{

									N = index - n ;
									E = index + n ;
									W = index + 1 ;
									S = index - 1 ;

									if(N>=0 && N<s)
									{
										val1 = I[N];
									}
									else
									{
										val1 = 0;
									}

									if(E>=0 && E<s)
									{
										val2 = I[E];
									}
									else
									{
										val2 = 0;
									}

									if(W>=0 && W<s)
									{
										val3 = I[W];
									}
									else
									{
										val3 = 0;
									}

									if(S>=0 && S<s)
									{
										val4 = I[S];
									}
									else
									{
										val4 = 0;
									}


									

									if(val1==label||val2==label||val3==label||val4==label)
									{
										I[index]  = label;
										a[k] = 1;
									}
									


								}
							}



							px = xo + i;
							py = yo - j;

							if (px >= 0 && px < m && py >= 0 && py < n)
							{
								index = (n)*px + py;

								if (I[index]  == 0)
								{

									N = index - n ;
									E = index + n ;
									W = index + 1 ;
									S = index - 1 ;

									if(N>=0 && N<s)
									{
										val1 = I[N];
									}
									else
									{
										val1 = 0;
									}

									if(E>=0 && E<s)
									{
										val2 = I[E];
									}
									else
									{
										val2 = 0;
									}

									if(W>=0 && W<s)
									{
										val3 = I[W];
									}
									else
									{
										val3 = 0;
									}

									if(S>=0 && S<s)
									{
										val4 = I[S];
									}
									else
									{
										val4 = 0;
									}



									
									if(val1==label||val2==label||val3==label||val4==label)
									{
										I[index]  = label;
										a[k] = 1;
									}
									


								}
							}


							px = xo - i;
							py = yo - j;

							if (px >= 0 && px < m && py >= 0 && py < n)
							{
								index = (n)*px + py;

								if (I[index]  == 0)
								{

									N = index - n ;
									E = index + n ;
									W = index + 1 ;
									S = index - 1 ;

									if(N>=0 && N<s)
									{
										val1 = I[N];
									}
									else
									{
										val1 = 0;
									}

									if(E>=0 && E<s)
									{
										val2 = I[E];
									}
									else
									{
										val2 = 0;
									}

									if(W>=0 && W<s)
									{
										val3 = I[W];
									}
									else
									{
										val3 = 0;
									}

									if(S>=0 && S<s)
									{
										val4 = I[S];
									}
									else
									{
										val4 = 0;
									}



									
									if(val1==label||val2==label||val3==label||val4==label)
									{
										I[index]  = label;
										a[k] = 1;
									}
									


								}
							}

							

						}

					}
				}

			}

		}
		complete = iscomplete(I);
		r = r + 1;
	}

	cout << "Execution Time of Evolution: " << double(clock() - t1) / CLOCKS_PER_SEC << endl;
	t1 = clock();
	ofstream fout;
	fout.open("Data.csv");
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n-1; ++j)
		{
			int index = (n)*i + j;
			fout<<I[index]<<",";
		}
		fout<<I[n*i + n-1]<<"\n";
		//fout<<endl;
	}
	fout.close();
	cout << "Execution Time of Writing CSV: " << double(clock() - t1) / CLOCKS_PER_SEC << endl;
	return 0;
}