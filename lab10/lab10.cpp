#include <iostream>
#include <fstream>
#include<string.h>
#include <time.h>
#include <sys/resource.h>

using namespace std;


struct str
{
	int date_of_birth;
	int ssn;
	string first_name;
	string last_name;
	string residence;
	int zip_code;
};

void print_array(str personal_data[], int const arr_size)
{
	for(int i =0; i<arr_size; i++)
	{
		cout<<personal_data[i].date_of_birth<<" "<<personal_data[i].ssn<<" "<<personal_data[i].first_name<<" "<<personal_data[i].last_name<<" ";
		cout<<personal_data[i].residence<<" "<<personal_data[i].zip_code<<endl;
	}
}

void print_str(str personal_data[], int const i)
{
	cout<<personal_data[i].date_of_birth<<" "<<personal_data[i].ssn<<" "<<personal_data[i].first_name<<" "<<personal_data[i].last_name<<" ";
        cout<<personal_data[i].residence<<" "<<personal_data[i].zip_code<<endl;
}

void search_array(str personal_data[], int const arr_size)
{
	cout<< "Imput the first and last name of the person you are trying to search: ";
	string first_name, last_name;
	cin>>first_name>>last_name;
	cout<<endl;
	for(int i =0; i<arr_size; i++)
	{
		if(first_name == personal_data[i].first_name||last_name == personal_data[i].last_name)
		{
			print_str(personal_data,i);
			break;
		}
	}
}

void find_oldest(str personal_data[], int const arr_size)
{
	int oldest_index = 0;
        for(int i =1; i<arr_size; i++)
        {
                if(personal_data[oldest_index].date_of_birth > personal_data[i].date_of_birth)
                {
                        oldest_index = i;
                }
        }
	print_str(personal_data,oldest_index);
}

int find_youngest(str personal_data[],int const a, int const b)
{
        int youngest_index = a;
        for(int i =a; i<b; i++)
        {
                if(personal_data[youngest_index].date_of_birth < personal_data[i].date_of_birth)
                {
                        youngest_index = i;
                }
        }
	return youngest_index;
}

void swap(str personal_data[],int index_a, int index_b)
{
	str temporary;
	temporary = personal_data[index_a];
	personal_data[index_a] = personal_data[index_b];
	personal_data[index_b] = temporary;
}

void sort_increasing_age(str personal_data[], int const arr_size)
{
	int youngest;
	for(int i = 0 ; i < arr_size ; i++)
	{
		youngest = find_youngest(personal_data,i,arr_size);
		swap(personal_data,youngest,i);
	}
}
double get_cpu_time()
{
	struct rusage ruse;
        getrusage(RUSAGE_SELF, &ruse);
        return ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec/1000000.0 + ruse.ru_stime.tv_sec+ruse.ru_stime.tv_usec/1000000.0;
}

void finData(str personal_data[], const int arr_size, const string s){
	string file;
	cout<<"To use default file enter d.\n";
	cin>>file;
	if(file == "d") file = "lab10_data.txt";
	else file = "/home/www/class/een118/labs/161/data" + s + ".txt";
	
	ifstream fin(file.c_str());
        if(fin.fail())
        {
                cout<< " the file could not be oppened\n";
        }
        else
        {
                for(int i =0;true;i++)
                {
                        if(fin.fail()) break;
                        fin>>personal_data[i].date_of_birth>>personal_data[i].ssn>>personal_data[i].first_name;
                        if(fin.fail()) break;
                        fin>>personal_data[i].last_name>>personal_data[i].residence>>personal_data[i].zip_code;

                }
        }
        fin.close();
}

void foutData(str personal_data[], const int arr_size, const string s){
	//this function creates a .text
	string file = "lab10_data_fout.txt";
        ofstream fout(file.c_str());
        if(!fout)
        {
                cout<<"can't create "<<file<<" output file!"<<endl;
        }
        else
        {
                for(int i = 0; i<arr_size; i++)
                {
                        fout<<personal_data[i].date_of_birth<<" "<<personal_data[i].ssn<<" "<<personal_data[i].first_name<<" ";
			fout<<personal_data[i].last_name<<" "<<personal_data[i].residence<<" "<<personal_data[i].zip_code<<endl;
                }
                cout << file<<" created" << endl;
                fout.close();
        }
}

void main()
{
	cout<<"Imput a file size to calculate [1|2|3|5|10|20|50|100](for default enter 1)\n";
	cout<<"file size : ";
	string size;
	cin>>size;
	int arr_size= atoi(size.c_str())*1000;
	str personal_data[arr_size];
	finData(personal_data,arr_size,size);	
	
	double start_time = get_cpu_time();
	sort_increasing_age(personal_data, arr_size);
	double end_time = get_cpu_time();
	foutData(personal_data,arr_size,size);
	cout<<"start time : "<<start_time<<" ; end time : "<< end_time<<endl;
	
}

/*
	List of CPU Times

| amount|initial|final  |

 1000 | 0.00581 | 0.0358224
 2000 | 0.008024| 0.127509
 5000 | 0.016469| 0.77067
10000 | 0.030416| 3.33875
20000 | 0.058142| 21.6176
50000 | 0.128906| 168.229
100000|
*/

/*
        List of CPU Times

| amount|initial|final       | differnce | (n-(n-1))/n  

Here I used strings in my structs to store the date of birth, ssn, zip
 1000 | 0.00581 | 0.0358224  | 0.0300124 | 0%
 2000 | 0.008024| 0.127509   | 0.119485  | 0.74881868%
 5000 | 0.016469| 0.77067    | 0.754201  | 0.84157063%
10000 | 0.030416| 3.33875    | 3.308334  | 0.77202997%
20000 | 0.058142| 21.6176    | 21.559458 | 0.846548269%
50000 | 0.128906| 168.229    | 168.10009 | 0.871746303%
100000| 0.274555| 701.145    | 700.870445| 0.76015526%



In lab10_1.cpp I used int to store the data and it is a lot faster

        list of CPU times

amount*1000| initial |  final
  1000 |0.005941 |0.008989
  2000 |0.00911  |0.019971
  5000 |0.018798 |0.081028
 10000 |0.034763 |0.273655
 20000 |0.067439 |1.02328
 500000|0.162829 |6.08857
100000 |0.323261 |25.9875
//*/
