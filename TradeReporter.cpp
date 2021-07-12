#include <fstream>
#include <sstream>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<set>
#include<map>
#include<set>
#include<limits>
#include<climits>
#include<cmath>
#include<algorithm>
#include <iomanip>

using namespace std;

struct symbolInfo {
    long long prev_time = 0;
    long long curr_max_time_gap = 0;
    long long tot_vol_traded;
    int max_trade_price = 0;
    double tot_price_quant;
    
};

class Parser {
public:

    static void getData(map<string, symbolInfo> & tradeData, string fileName) {
        long long ts;
        string symbol;
        int quantity;
        int price;
        ifstream file(fileName);
        string   line;
        while (getline(file, line))
        {
            stringstream   linestream(line);
            string         data;

            int st = 0;
            while (getline(linestream, data, ',')) {
                if (st == 0) ts = stoll(data);
                else if (st == 1) symbol = data;
                else if (st == 2) quantity = stoi(data);
                else if (st == 3) price = stoi(data);     
                st++;
            }

            tradeData[symbol].curr_max_time_gap = tradeData[symbol].prev_time > 0 ? max(ts - tradeData[symbol].prev_time, tradeData[symbol].curr_max_time_gap) : 0;
            tradeData[symbol].tot_vol_traded += quantity;
            tradeData[symbol].max_trade_price = max(price, tradeData[symbol].max_trade_price);
            tradeData[symbol].tot_price_quant += ((double) price * quantity);
            tradeData[symbol].prev_time = ts;             
        }
    }
};

class Writer {
public:
    static void writeData(map<string, symbolInfo>& tradeData, string file) {
        ofstream outFile;
        outFile.open(file);        
        for (auto p : tradeData) {
            outFile << p.first << "," << p.second.curr_max_time_gap << "," << p.second.tot_vol_traded << "," << (long long)p.second.tot_price_quant / p.second.tot_vol_traded   << "," << p.second.max_trade_price  << "\n";
        }

    }
};

class Test {
public:
    static bool testData(string fileNameIn) {
        map<string, symbolInfo> data;
        Parser p;
        Writer w;
        string fileNameOut = "./tempFile.csv" ;
        p.getData(data, fileNameIn);
        w.writeData(data, fileNameOut);
        ifstream file(fileNameOut);
        string line;
        vector<string> result;
        while (getline(file, line)) {
            result.push_back(line);
        }        
        vector<string> expResult = { "aaa,5787,40,1161,1222", "aab,6103,69,810,907", "aac,3081,41,559,638","abb,0,9,756,756"};
        return result == expResult;
    }
};

int main()
{
    map<string, symbolInfo> data;  
    Test t;
    bool chk = t.testData("./input2.csv");
    if (!chk) {
        cout << "Test failed\n";
    }
    Parser p;
    Writer w;
    p.getData(data, "./input.csv");
    w.writeData(data, "./output.csv");

}
