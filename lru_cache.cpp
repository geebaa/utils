#include <bits/stdc++.h>

using namespace std;
// comparator class for min_heap using priority queue
class Compare
{
public:
    bool operator() (pair<unsigned long long , int> &v1, pair<unsigned long long , int> &v2)
    {
        return v1.first > v2.first;
    }
};
class LRUCache {
private:
    int n; // Max number of elements in cache
    unordered_map<int,pair<unsigned long long , int> > kv; // map of key to a pair <time,val>
    // min_heap implementation for LRU
    priority_queue<pair<unsigned long long , int >,vector < pair<unsigned long long , int > >,Compare> min_heap;
    unordered_map<int,pair<unsigned long long , int> >::iterator itr;
public:
    // keeps track of time and rollover of time not handled by
    // this class. Rollover of time requires a rebuild of the existing map "kv" and priority_queue min_heap
    static unsigned long long curr_time;
    LRUCache(int capacity) {
        n=capacity;
        LRUCache::curr_time=0;
    }
    
    // O(1) get
    // broadly takes care of 2 conditions
    // 1) element in cache
    // 2) element not in cache
    int get(int key) {
        itr=kv.find(key);
        pair<unsigned long long , int> val;
        if(itr==kv.end()){
            return -1;
        }else{
            //update timestamp
            itr->second.first = LRUCache::curr_time++;
            val=itr->second;
            return val.second;
        }
    }
    
    // O(log(n)) put
    // boardly takes care of below 3 conditions
    // 1) key exists , 
    // 2) key does not exist and cache is full 
    // 3) key does not exist and cache has space.
    void put(int key, int value) {
        itr=kv.find(key);

        if(itr != kv.end()){
            // if key already exisits just update value and timestamp in the map
            itr->second.first = LRUCache::curr_time++;
            itr->second.second = value;

        }else if(kv.size() == n){
            // find a replacement
            pair<unsigned long long , int> pq_top;
            pair<unsigned long long , int> map_kv;
            pq_top = min_heap.top();
            map_kv=kv[pq_top.second];

            while(pq_top.first < map_kv.first){
                // we had a get that is after the put. update the ts in min_pq
                // pop the element and push with new timestamp
                min_heap.pop();
                pq_top.first=map_kv.first;
                min_heap.push(pq_top);
                // check the current
                pq_top = min_heap.top();
                map_kv=kv[pq_top.second];
            }

            // LRU is pq_top, remove the element from min_heap
            min_heap.pop();

            // remove the old element from map
            kv.erase(pq_top.second);

            // update the time for new values
            unsigned long long time;
            time = LRUCache::curr_time++;

            // push the new element into min_heap
            pq_top.first=time;
            pq_top.second=key;
            min_heap.push(pq_top);

            // insert the new element into map
            pair<unsigned long long , int> ts_val;
            ts_val.first=time;
            ts_val.second=value;
            kv[key]=ts_val;

        }else{
            // insert into min_heap and map
            pair<unsigned long long , int> ts_val;
            pair<unsigned long long , int> ts_key;
            unsigned long long time;
            time = LRUCache::curr_time++;
            ts_val.first=time;
            ts_val.second=value;
            ts_key.first=time;
            ts_key.second=key;

            kv[key]=ts_val;
            min_heap.push(ts_key);
        }
    }
};
