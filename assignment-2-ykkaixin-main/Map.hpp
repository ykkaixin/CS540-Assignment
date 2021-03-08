#include<iostream>
#include<utility>
#include<string.h>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<stdexcept>
#define levels 100
namespace cs540{

template <typename K, typename M>
class Map;

template <typename K, typename M>
class Node{
public:
    friend class Map<K, M>;
    typedef std::pair<const K, M> ValueType;

    Node(std::size_t l ){
        int temp = l +1;
        forward = new Node*[temp];
        size_t size = sizeof(Node*)*(temp);
        memset(forward, '\0', size);
        value = NULL;
        prev = NULL;
    }

    Node(std::size_t l , const ValueType& val){
        int temp = l +1;
        forward = new Node*[temp];
        size_t size = sizeof(Node*)*(temp);
        memset(forward, '\0', size);
        value = new std::pair<const K, M>(val);
        prev = NULL;
    }

    //delete forward links and value
    ~Node(){
        delete [] forward;
        delete  value;
    }

private:
    Node** forward;
    Node* prev;
    ValueType * value;
};

template <typename K, typename M>
class Map{
public:
    //member type
    typedef std::pair<const K, M> ValueType;

    /*
    defining Iterator class
    helps iterate through elements of the map
    */
    class Iterator{
    public:
        Iterator(const Iterator& it) : cur(it.getCurrent()){}
        Iterator(Node<K, M>* _node) : cur(_node){}
        Node<K, M>* getCurrent() const{return cur;}

        //defining operators for Iterator
        Iterator& operator = (const Iterator& it){
            cur = it.getCurrent();
            return *this;
        }

        Iterator& operator++(){
            if(cur == NULL) return *this;
            cur = cur->forward[0];
            return *this;
        }

        Iterator operator++(int){
            Map<K, M>::Iterator _this = *this;
            if(cur == NULL) return _this;
            cur = cur->forward[0];
            return _this;
        }

        Iterator& operator--(){
            if(cur == NULL) return *this;
            cur = cur->prev;
            return *this;
        }

        Iterator operator--(int){
            Map<K, M>::Iterator _this = *this;
            if(cur == NULL) return _this;
            cur = cur->prev;
            return _this;
        }

        ValueType& operator*() const{
            return *cur-> value;
        }

        ValueType* operator->() const{
            return cur-> value;
        }

        ~Iterator(){
            cur = nullptr;
        }

    private:
        Node<K, M> *cur;

    };


    class ConstIterator{
    public:
        ConstIterator(const ConstIterator& c) : cur(c.getCurrent()){}
        ConstIterator(const Iterator& it) : cur(it.getCurrent()){}
        ConstIterator(Node<K, M> *_node) : cur(_node){}
        Node<K, M>* getCurrent() const{return cur;}

        //defining operators for ConstIterator
        ConstIterator& operator=(const ConstIterator& c){
            cur = c.getCurrent();
            return *this;
        }
        ConstIterator& operator++(){
            if(cur == NULL) return *this;
            cur = cur->forward[0];
            return *this;
        }
        ConstIterator operator++(int){
            Map<K, M>::ConstIterator _this = *this;
            if(cur == NULL) return _this;
            cur = cur->forward[0];
            return _this;
        }
        ConstIterator& operator--(){
            if(cur == NULL) return *this;
            cur = cur->prev;
            return *this;
        }
        ConstIterator operator--(int){
            Map<K, M>::ConstIterator _this = *this;
            if(cur == NULL) return _this;
            cur = cur->prev;
            return _this;
        }
        const ValueType& operator*() const{
            return *cur-> value;
        }
        const ValueType* operator->() const{
            return cur-> value;
        }

        ~ConstIterator(){
            cur = nullptr;
        }

    private:
        Node<K, M> *cur;
    };

    class ReverseIterator{
    public:
        ReverseIterator(const ReverseIterator& rit) : curr(rit.getCurrent()){}
        ReverseIterator(Node<K, M> *_node) : curr(_node){}
        Node<K, M>* getCurrent() const{return curr;}

        //defining operators for ReverseIterator
        ReverseIterator& operator=(const ReverseIterator& rit){
            curr = rit.getCurrent();
            return *this;
        }
        ReverseIterator& operator++(){
            if(curr == NULL) return *this;
            curr = curr->prev;
            return *this;
        }
        ReverseIterator operator++(int){
            Map<K, M>::ReverseIterator _this = *this;
            if(curr == NULL) return _this;
            curr = curr->prev;
            return _this;
        }
        ReverseIterator& operator--(){
            if(curr == NULL) return *this;
            curr = curr->forward[0];
            return *this;
        }
        ReverseIterator operator--(int){
            Map<K, M>::ReverseIterator _this = *this;
            if(curr == NULL) return _this;
            curr = curr->prev;
            return _this;
        }
        ValueType& operator*() const{
            return *curr->value;
        }
        ValueType* operator->() const{
            return curr->value;
        }

        ~ReverseIterator(){
            curr = nullptr;
        }

    private:
        Node<K, M> *curr;
    };

    Map(){
        map_ctor();
    }

    Map(const Map& m){
        map_ctor();
        Node<K, M> *temp = m.get_head()->forward[0];
        while(temp != m.get_tail()){
            ValueType& val = *(temp-> value);
            insert(val);
            temp = temp->forward[0];
        }
    }

    Map(std::initializer_list<std::pair<const K, M>> l){
        map_ctor();
        auto it = l.begin();
        while(it != l.end()){
            insert(*it);
            it++;
        }
    }

    //delete every node in the map
    ~Map(){
        Node<K, M> *_temp_head = head;
        Node<K, M> *_temp;
        while(_temp_head != NULL){
            _temp = _temp_head->forward[0];
            delete _temp_head;
            _temp_head = _temp;
        }
    }

    Node<K, M>* get_head() const{
        return head;
    }
    Node<K, M>* get_tail() const{
        return tail;
    }

    //constructs the map
    void map_ctor(){
        init_head_tail();
        init_assign_head_tail();
        init_size();
    }
    //init parameters
    void init_head_tail(){
        head = new Node<K, M>(levels);
        tail = new Node<K, M>(levels);
    }
    void init_assign_head_tail(){
        head->forward[0] = tail;
        tail->prev = head;
        head->prev = NULL;
        tail->forward[0] = NULL;
    }
    void init_size(){
        max = 0;
        _size = 0;
    }

    //reset parameters
    void reset_head_tail(){
        head = NULL;
        tail = NULL;
    }
    void reset_size(){
        max = 0;
        _size = 0;
    }


    /*
    begin() -- return iterator pointing to the first element in the map
    end() -- return iterator pointing to one past the last element in the map (logically)
    rbegin() -- works like begin() for ReverseIterator
    rend() -- works like end() for ReverseIterator
    */
    Iterator begin(){return Map<K, M>::Iterator(head->forward[0]);}
    Iterator end(){return Map<K, M>::Iterator(tail);}
    ConstIterator begin() const{return ConstIterator(head -> forward[0]);}
    ConstIterator end() const{return ConstIterator(tail);}
    ReverseIterator rbegin(){return ReverseIterator(tail -> prev);}
    ReverseIterator rend(){return ReverseIterator(head);}

    std::size_t size() const{return _size;}


    //return mapped type for the given key
    M& at(const K& key){
        Node<K, M> *temp = findAtBottom(key);
        if(temp == NULL){
            throw std::out_of_range("out of range");
        }
        else return temp -> value->second;

    }

    const M& at(const K& k) const{
        Node<K, M> *temp = findAtBottom(k);
        if(temp == NULL){
            throw std::out_of_range("out of range");
        }
        else return temp->value->second;
    }


    //comparison operators on iterator
    friend bool operator==(const Iterator& _iter1, const Iterator& _iter2){
        return (_iter1.getCurrent() == _iter2.getCurrent())? true : false;
    }
    friend bool operator!=(const Iterator& _iter1, const Iterator& _iter2){
        return (_iter1.getCurrent() != _iter2.getCurrent())? true : false;
    }
    friend bool operator==(const ConstIterator& _citer1, const ConstIterator& _citer2){
        return (_citer1.getCurrent() == _citer2.getCurrent())? true : false;
    }
    friend bool operator!=(const ConstIterator& _citer1, const ConstIterator& _citer2){
        return (_citer1.getCurrent() != _citer2.getCurrent())? true : false;
    }
    friend bool operator==(const Iterator& it, const ConstIterator& c){
        return (it.getCurrent() == c.getCurrent())? true : false;
    }
    friend bool operator==(const ConstIterator& c, const Iterator& it){
        return (c.getCurrent() == it.getCurrent())? true : false;
    }
    friend bool operator!=(const Iterator& it, const ConstIterator& c){
        return (it.getCurrent() != c.getCurrent())? true : false;
    }
    friend bool operator!=(const ConstIterator& c, const Iterator& it){
        return (c.getCurrent() != it.getCurrent())? true : false;
    }
    friend bool operator==(const ReverseIterator& rit, const ReverseIterator& rit2){
        return (rit.getCurrent() == rit2.getCurrent())? true : false;
    }
    friend bool operator!=(const ReverseIterator& rit, const ReverseIterator& rit2){
        return (rit.getCurrent() != rit2.getCurrent())? true : false;
    }

    //operator[] for accessing mapped type for key
    M& operator[](const K&);
    //level randomizer for insert
    size_t randomLevel(Node<K, M>**);
    //getting node for key using bottom link
    Node<K, M>* findAtBottom(const K& key) const;
    //insert ValueType into map and return iterator positioned at that node
    std::pair<Iterator, bool> insert(const ValueType&);
    void erase(Iterator);
    //remove node which has given key
    void erase(const K&);
    //return Iterator pointing to the node which has key given by key
    Iterator find(const K& key){
        Node<K, M> *_temp_head = findAtBottom(key);
        if(_temp_head == NULL){
            return Map<K, M>::Iterator(tail);
        }
        return Map<K, M>::Iterator(_temp_head);
    }

    //assignment of maps
    Map& operator=(const Map&);
    //map comparisons
    bool operator!=(const Map&) const;
    template <typename Key, typename Mapped>
    friend bool operator==(const Map<Key, Mapped>&, const Map<Key, Mapped>&);
    template <typename Key, typename Mapped>
    friend bool operator<(const Map<Key, Mapped>&, const Map<Key, Mapped>&);
    template <typename Key, typename Mapped>
    friend bool operator!=(const Map<Key, Mapped>&, const Map<Key, Mapped>&);

private:
    Node<K, M>* head;
    Node<K, M>* tail;
    std::size_t max;
    std::size_t _size;
};

template <typename K, typename M>
M& Map<K, M>::operator[](const K& key){
    Node<K, M> *_temp_head = findAtBottom(key);
    if(_temp_head == NULL){
        Map<K, M>::Iterator it = insert(std::make_pair(key, M())).first;
        return it.getCurrent()-> value->second;
    }
    return _temp_head-> value->second;
}

template <typename K, typename M>
size_t Map<K, M>::randomLevel(Node<K, M>** _nodes){
    size_t _level = 0;
    while(_level < levels && rand() < RAND_MAX*0.5){
        ++_level;
    }
    if(_level > max){
        size_t _temp = max+1;
        while(_temp <= _level){
            _nodes[_temp] = head;
            ++_temp;
        }
        max = _level;
    }
    return _level;
}

template <typename K, typename M>
Map<K, M>& Map<K, M>::operator=(const Map<K, M>& m){
    if(this == &m){
        return *this;
    }
    Node<K, M> *_temp_head = head;
    Node<K, M> *_temp;
    while(_temp_head != NULL){
        _temp = _temp_head->forward[0];
        delete _temp_head;
        _temp_head = NULL;
        _temp_head = _temp;
    }

    map_ctor();
    Node<K, M> *_first = m.get_head()->forward[0];
    if(_first == m.get_tail()) return *this;
    while(_first != m.get_tail()){
        insert(*(_first-> value));
        _first = _first->forward[0];
    }
    return *this;
}

template <typename K, typename M>
std::pair<typename Map<K, M>::Iterator, bool> Map<K, M>::insert(const std::pair<const K, M>& val){
    const K& key = val.first;
    Node<K, M> *_temp = head;
    Node<K, M> **u = new Node<K, M>* [levels+1];
    memset(u, '\0', sizeof(Node<K, M>*)*(levels+1));
    size_t i = max;
    while(i >= 1){
        while(_temp->forward[i] != NULL && _temp->forward[i]-> value->first < key){
            _temp = _temp->forward[i];
        }
        u[i] = _temp;
        i--;
    }

    while(_temp->forward[0] != tail && _temp->forward[0]-> value->first < key){
        _temp = _temp->forward[0];
    }
    u[0] = _temp;
    Node<K, M> *_first_updated = u[0];
    _first_updated = _first_updated->forward[0];
    if(_first_updated-> value != NULL && _first_updated-> value->first == val.first){
        delete [] u;
        return std::make_pair(Map<K, M>::Iterator(_first_updated), false);
    }

    size_t _level = randomLevel(u);
    //_first_updated = NULL;
    _first_updated = new Node<K, M>(_level, val);
    i = 0;
    while(i <= _level){
        _first_updated->forward[i] = u[i]->forward[i];
        u[i]->forward[i] = _first_updated;
        ++i;
    }
    _first_updated->prev = u[0];
    if(_first_updated->forward[0] != tail){
        _first_updated->forward[0]->prev = _first_updated;
    }
    else{
        tail->prev = _first_updated;
    }
    ++_size;
    delete [] u;
    return std::make_pair(Map<K, M>::Iterator(_first_updated), true);
}



template <typename K, typename  M>
Node<K, M>* Map<K, M>::findAtBottom(const K& key) const{
    Node<K, M> *_temp = head;
    int i = max;
    while(i >= 1){
        while(_temp->forward[i] != NULL && _temp->forward[i]-> value->first < key){
            _temp = _temp->forward[i];
        }
        i--;
    }

    while(_temp->forward[0] != tail && _temp->forward[0]-> value->first < key){
        _temp = _temp->forward[0];
    }
    _temp = _temp->forward[0];
    if(_temp == tail) return NULL;
    if(_temp != NULL){
        if(_temp-> value->first == key)
            return _temp;
    }
    return NULL;
}
template <typename K, typename M>
void Map<K, M>::erase(Map<K, M>::Iterator it){
    const K& key = it.getCurrent()->value->first;
    Node<K, M> *_temp = head;
    Node<K, M> **u = new Node<K, M>* [levels+1];
    memset(u, '\0', sizeof(Node<K, M>*)*(levels+1));
    size_t i = max;
    while(i >= 1){
        while(_temp->forward[i] != NULL && _temp->forward[i]->value->first < key){
            _temp = _temp->forward[i];
        }
        u[i] = _temp;
        i--;
    }

    while(_temp->forward[0] != tail && _temp->forward[0]->value->first < key){
        _temp = _temp->forward[0];
    }
    u[0] = _temp;
    Node<K, M> *_first_updated = u[0];
    _first_updated = _first_updated->forward[0];
    if(_first_updated->value->first == key){
        i = 0;
        while(i <= max && u[i]->forward[i] == _first_updated){
            u[i]->forward[i] = _first_updated->forward[i];
            ++i;
        }

        if(_first_updated->forward[0] != tail){
            _first_updated->forward[0]->prev = _first_updated->prev;
        }
        else{
            tail->prev = _first_updated->prev;
            _first_updated->prev->forward[0] = tail;
        }

        delete _first_updated;
        while(max > 0 && head->forward[max] == NULL){
            --max;
        }
        --_size;
        delete [] u;
    } else{
        throw std::out_of_range("out of range");
    }
}
template <typename K, typename M>
void Map<K, M>::erase(const K& key){
        Node<K, M> *temp = head;
        Node<K, M> **u = new Node<K, M>* [levels+1];
        memset(u, '\0', sizeof(Node<K, M>*)*(levels+1));
        size_t i = max;
        while(i >= 1){
            while(temp->forward[i] != NULL && temp->forward[i]->value->first < key){
                temp = temp->forward[i];
            }
            u[i] = temp;
            i--;
        }

        while(temp->forward[0] != tail && temp->forward[0]->value->first < key){
            temp = temp->forward[0];
        }
        u[0] = temp;
        Node<K, M> *_first_updated = u[0];
        _first_updated = _first_updated->forward[0];
        if(_first_updated->value->first == key){
            i = 0;
            while(i <= max && u[i]->forward[i] == _first_updated){
                u[i]->forward[i] = _first_updated->forward[i];
                ++i;
            }

            if(_first_updated->forward[0] != tail){
                _first_updated->forward[0]->prev = _first_updated->prev;
            }
            else{
                tail->prev = _first_updated->prev;
                _first_updated->prev->forward[0] = tail;
            }

            delete _first_updated;
            while(max > 0 && head->forward[max] == NULL){
                --max;
            }
            --_size;
            delete [] u;
        }
        else{
            throw std::out_of_range("out of range");
        }
    }

template <typename K, typename M>
bool Map<K, M>::operator!=(const Map<K, M>& m) const{
    if(*this == m) return false;
    return true;
}

template <typename K, typename M>
bool operator==(const Map<K, M>& _map1, const Map<K, M>& _map2){
    if(_map1.size() != _map2.size()) return false;
    auto _iter1 = _map1.begin();
    auto _iter2 = _map2.begin();
    while(_iter1 != _map1.end() && _iter2 != _map2.end()){
        if(*_iter1 != *_iter2){
            return false;
        }
        ++_iter1;
        ++_iter2;
    }
    return true;
}

template <typename K, typename M>
bool operator!=(const Map<K, M>& _map1, const Map<K, M>& _map2){
                                                           return !(_map1 == _map2);
                                                           }

template <typename K, typename M>
bool operator<(const Map<K, M>& _map1, const Map<K, M>& _map2){
    size_t _size1 = _map1.size();
    size_t _size2 = _map2.size();
    if(_size1 < _size2) return true;
    if(_size2 < _size1) return false;
    //same size
    auto _iter1 = _map1.begin();
    auto _iter2 = _map2.begin();
    while(_iter1 != _map1.end() && _iter2 != _map2.end()){
        bool _less = (*_iter1).first < (*_iter2).first;
        bool _less2 = (*_iter2).first < (*_iter1).first;
        if(_less) return true;
        if(_less2) return false;
        ++_iter1;
        ++_iter2;
    }
    //maps are same
    return false;
}
}

