#include "embedjoin.h"


int k_input = 50;
int num_str =5; //r
int num_hash = 5; //z
int num_char = 4;// num of chars in strings
int num_bit = 15; //m
int samplingrange; //the maximum digit to embed, the range to sample
string filename;
int outputresults;
int shift=10000000;
int num_rep;
int countfilter=0;

//size of hash table;
int M = 1000003;
vector<int> a; // the random vector for second level hash table

int *hash_eb;
vector<vector<int>> hash_lsh;
vector<int> lshnumber;
vector<string> oridata; //original strings
vector<int> indices;
vector<vector<vector<string>>> embdata;// embedded strings
vector<string> oridata_modified; //modified strings for edit distance computation
vector<idpair> outputs;


void setuplsh()
{
	vector<vector<int>> inihash_lsh(num_hash, vector<int>(num_bit));
	hash_lsh = inihash_lsh;

	for (int i = 0; i < num_hash; i++)
	for (int j = 0; j < num_bit; j++)
		hash_lsh[i][j] = rand() % (samplingrange);

	for (int i = 0; i < num_bit; i++)
		a.push_back(rand() % (M - 1));


	lshnumber.reserve(hash_lsh.size());
	for (int i = 0; i < num_hash; i++)
		lshnumber.insert(lshnumber.end(), hash_lsh[i].begin(), hash_lsh[i].end());
	sort(lshnumber.begin(), lshnumber.end());
	lshnumber.erase(unique(lshnumber.begin(), lshnumber.end()), lshnumber.end());
	samplingrange = lshnumber[lshnumber.size() - 1];

	for (int i = 0; i < num_hash; i++)
	for (int j = 0; j < num_bit; j++)
		hash_lsh[i][j] = lower_bound(lshnumber.begin(), lshnumber.end(), hash_lsh[i][j]) - lshnumber.begin();

}

inline int dict(const char& input)
{
	if (num_char == 26 || num_char == 25)
	{
		return input - 'A';
	}
	else if (num_char == 37)
	{
		if (input >= 'A' && input <= 'Z')
			return input - 'A';
		if (input >= '0' && input <= '9')
			return 26 + input - '0';
		if (input == ' ')
			return 36;
		else
			fprintf(stderr, "input error: check the dictory of your input\n");
	}
	else if (num_char == 5 || num_char == 4)
	{
		if (input == 'A')
			return 0;
		else if (input == 'C')
			return 1;
		else if (input == 'G')
			return 2;
		else if (input == 'T')
			return 3;
		else if (input == 'N')
			return 4;
		else
			fprintf(stderr, "input error: check the dictory of your input\n");
	}
	else
		fprintf(stderr, "input error: check the dictory of your input\n");
}

inline string embedstr(int id, int strid, int **p, int pos = 0)
{

	string output;
	int i = pos, partdigit = 0;
	int size = oridata[id].size();

	for (int j = 0; i < size && j <= samplingrange; i++)
	{
		char s = oridata[id][i];

		j += *(p[dict(s)] + j) + 1;

		//only record the bits used in LSH
		while (partdigit< lshnumber.size() && j > lshnumber[partdigit])
		{
			output += s;
			partdigit++;
		}
	}
	return output;
}

vector<vector<vector<string>>> embeddata()
{
	vector<vector<vector<string>>> output(oridata.size(), vector<vector<string>>(num_str, vector<string>(num_rep, "")));

	int **p;
	p = new int *[num_char];
	hash_eb = new int[num_char * (samplingrange + 1)];

	for (int j = 0; j < num_str; j++)
	{

		for (int t = 0; t < num_char; t++)
		{
			p[t] = &hash_eb[t*(samplingrange + 1)];
			for (int d = 0; d < samplingrange + 1; d++)
			{
				hash_eb[t*(samplingrange + 1) + d] = 1-rand() % 2;
			}

			for (int d = 0; d < samplingrange + 1; d++)
			{
				if (hash_eb[t*(samplingrange + 1) + d] == 1)
				{
					if (d>0 && hash_eb[t*(samplingrange + 1) + d - 1] == 1)
					{
						hash_eb[t*(samplingrange + 1) + d] = hash_eb[t*(samplingrange + 1) + d - 1] - 1;
					}
					else
					{
						int next = d + 1;
						while (next < samplingrange + 1 && hash_eb[t*(samplingrange + 1) + next] == 1)
						{
							hash_eb[t*(samplingrange + 1) + d]++;
							next++;
						}
					}
				}
			}
		}

		for (int i = 0; i < oridata.size(); i++)
		for (int k = 0; k < num_rep; k++)
		{
			output[i][j][k] = embedstr(i, j, p, shift*k);
		}

	}
	delete[] p;
	delete[] hash_eb;
	return output;
}

void readdata(string filename)
{
	ifstream  data(filename);
	string cell;
	int number_string = 0;

	while (getline(data, cell))
	{
		number_string++;
		oridata.push_back(cell);
	}
	compare c;

	for (int i = 0; i < oridata.size(); i++)
		indices.push_back(i);

	sort(indices.begin(), indices.end(), [&](int i1, int i2) { return oridata[i1].size() <  oridata[i2].size();});
	sort(oridata.begin(), oridata.end(), c);

	oridata_modified = oridata;		// append distinguisher at the end
	for (int j = 0; j < oridata_modified.size(); j++){ 
		for(int k = 0;k < 8; k++) oridata_modified[j].push_back(j>>(8*k));
	}

}

bool lsh(int id1, idpair id2, int t, int k, int q)
{
	for (int j = 0; j < num_bit; j++)
	{
		if (hash_lsh[k][j] < embdata[id1][t][q].size() && hash_lsh[k][j] < embdata[get<0>(id2)][t][get<1>(id2)].size())
		if (embdata[id1][t][q][hash_lsh[k][j]] != embdata[get<0>(id2)][t][get<1>(id2)][hash_lsh[k][j]])
			return false;
	}
	return true;
}

int findid(int i, int t, int k,int q)
{
	int id = 0;
	for (int j = 0; j < num_bit; j++)
	{
		if (hash_lsh[k][j]<embdata[i][t][q].size())
			id += (dict(embdata[i][t][q][hash_lsh[k][j]]))*a[j];
	}
	return id % M;
}

void initialization()
{
	/*read data*/
	recordtime();

	readdata(filename);

	updatetime(1);

	fprintf(stderr, "Read:%s \n", filename.c_str());
	fprintf(stderr, "Threshold:%d \n", k_input);
	fprintf(stderr, "Shift, Num of Rep:%d %d \n", shift, num_rep);
	if(countfilter)
		fprintf(stderr, "Count filter \n", shift, num_rep);
	fprintf(stderr, "r,z,m:%d %d %d \n", num_str, num_hash, num_bit);

	/*embed data*/
	recordtime();

	setuplsh();
	embdata = embeddata();

	updatetime(2);
}

void updatehash(list<idpair> &bucket, vector<idthree> &candidate, int i, int t, int k, int q)
{
	if (!bucket.empty())
	{
		for (list<idpair>::iterator it = bucket.begin(); it != bucket.end();)
		{
			//since sorted, we remove the instances than cannot be candidates any more(they are too short)
			if (abs(int(oridata[i].size() - oridata[get<0>(*it)].size()))>k_input)
			{
				it = bucket.erase(it);
			}
			else
			{
				if (lsh(i, (*it), t, k,q))
				if (i != get<0>(*it))
					candidate.push_back(make_tuple(get<0>(*it), get<1>(*it),q));
				++it;
			}
		}
	}
}


void join(int start = 0, int end = oridata.size())
{
	int num_candidate = 0;
	vector<vector <vector<list<idpair>>>> bucket(num_str, vector<vector<list<idpair>>>(num_hash, vector<list<idpair>>(M, list<idpair>())));

	for (int i = start; i < end; i++)
	{
		//lsh
		recordtime();

		vector<idthree> candidate;
		vector<int> verifycan;

		for (int t = 0; t < num_str; t++)
		for (int k = 0; k < num_hash; k++)
		for (int q = 0; q < num_rep; q++)
		{
			int id = findid(i, t, k, q);
			updatehash(bucket[t][k][id], candidate, i, t, k, q);
			bucket[t][k][id].push_back(make_tuple(i, q));
		}
		
		//remove dupulicate candidates
		sort(candidate.begin(), candidate.end());

		vector<int> freq_uv;
		if (candidate.size()>0)
		{
			freq_uv.push_back(0);
			auto prev = candidate[0];        // you should ensure !uv.empty() if previous code did not already ensure it.
			for (auto const & x : candidate)
			{
				if (prev != x)
				{
					freq_uv.push_back(0);
					prev = x;
				}
				++freq_uv.back();
			}
		}
		candidate.erase(unique(candidate.begin(), candidate.end()), candidate.end());

		for (int j = 0; j < candidate.size(); j++)
		{
			if (freq_uv[j] > countfilter )
			{
				verifycan.push_back(get<0>(candidate[j]));
			}
		}

		updatetime(3);
		recordtime();


		//remove dupulicate candidates
		sort(verifycan.begin(), verifycan.end());
		verifycan.erase(unique(verifycan.begin(), verifycan.end()), verifycan.end());

		//edit distance calculation
		for (int j = 0; j < verifycan.size(); j++)
		{
			num_candidate++;
			int ed = edit_distance(oridata_modified[verifycan[j]].data(), oridata[verifycan[j]].size(),oridata_modified[i].data(), oridata[i].size(), k_input);
			if(ed != -1)outputs.push_back(make_tuple(i, verifycan[j]));
		}

		updatetime(4); 
	}
	fprintf(stderr, "Num of candidate:%d\n", num_candidate);
}

void output()
{
	sort(outputs.begin(), outputs.end());
	outputs.erase(unique(outputs.begin(), outputs.end()), outputs.end());
	
	if(outputresults)
	for (int i = 0; i < outputs.size(); i++)
	{

		cout << indices[get<0>(outputs[i])] << " " << indices[get<1>(outputs[i])]<<endl;
		cout << oridata[get<0>(outputs[i])] << endl;
		cout << oridata[get<1>(outputs[i])] << endl;
	}

	fprintf(stderr, "Num of outputs:%d\n", int(outputs.size()));
}

int main(int argc, char **argv)
{
	if (argc >1) 
	{		
		filename = argv[1];
		k_input = atoi(argv[2]);
		num_str = atoi(argv[3]);
		num_hash = atoi(argv[4]);
		num_bit = atoi(argv[5]);
		num_char = atoi(argv[6]);
		samplingrange = atoi(argv[7]);
		outputresults = atoi(argv[8]);
		if (argc >9)
			shift= atoi(argv[9]);
		if (argc >10)
			countfilter= atoi(argv[10]);
		if(shift !=0)
			num_rep = ceil(double (k_input) / double (shift));
		//srand(time(NULL)); 
		srand(11110);
		initialization();
		join();
		output();
		outputtime();
	}
	else
	{
		fprintf(stderr, "usage: ./embedjoin inputdata K r z m dictsize embedrange outputallresults shift T \n");
	}
}



