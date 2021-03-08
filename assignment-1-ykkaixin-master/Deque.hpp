#ifndef DEQUE_H
#define DEQUE_H
#include<string.h>

#define Deque_DEFINE(t)    										                                        \
struct Deque_##t##_Iterator;                                                                   			\
struct Deque_##t                                                                            			\
{                                                                                           			\
    t * data;                                                                               			\
    char type_name[6 + sizeof(#t)] = "Deque_";                                                          \
    size_t curSize;                                                                            			\
    size_t capacity;                                                                           			\
    int insertFront;                                                                           			\
    int insertBack;                                                                            			\
    size_t (*size) (Deque_##t * deq);                                                              		\
    bool (*empty) (Deque_##t * deq);                                                            		\
    void (*push_back) (Deque_##t * deq, const t v);                                                 	\
    void (*push_front) (Deque_##t * deq, const t v);                                                   \
    t& (*front) (Deque_##t * deq);                                                              		\
    t& (*back) (Deque_##t * deq);                                                               		\
    void (*pop_front) (Deque_##t * deq);                                                        		\
    void (*pop_back) (Deque_##t * deq);                                                        			\
    Deque_##t##_Iterator (*begin) (Deque_##t * deq);                                            		\
    Deque_##t##_Iterator (*end) (Deque_##t * deq);                                              		\
    t& (*at) (Deque_##t * deq, size_t st);                                                         		\
    void (*clear) (Deque_##t * deq);                                                            		\
    void (*dtor) (Deque_##t * deq);                                                             		\
    bool (*less) (const t& var1, const t& var2);								                        \
    void (*sort) (Deque_##t *deq, Deque_##t##_Iterator front, Deque_##t##_Iterator end);                \
};                                                                                          			\
                                                                                            			\
struct Deque_##t##_Iterator                                                          			       	\
{                                                                                           			\
    int index;                                                                              			\
    Deque_##t * deq;                                                                        			\
    int toEnd;                                                                                          \
    void (*inc) (Deque_##t##_Iterator * it);                                                   			\
    t& (*deref) (Deque_##t##_Iterator * it);                                                   			\
    void (*dec) (Deque_##t##_Iterator * it);                                                   			\
};                                                                                          			\
                               											                                \
void Deque_##t##_swap(t* array, int i, int j) {                                         			    \
		t temp = array[i];                                                                  			\
		array[i] = array[j];                                                                			\
		array[j] = temp;                                                                    			\
}                                                                                       			    \
	                                                                                        			\
int Deque_##t##_partition(t* array, bool (*compare)(const t&, const t&), int b, int f) {			    \
    t pivot = array[f];																				    \
    int i = b-1;                                                                        			    \
    for(int j=b; j<=f-1; j++) {                                                         			    \
        if(!compare(array[j], pivot)) {                                                 			    \
            i++;                                                                        			    \
            Deque_##t##_swap(array, i, j);                                              			    \
        }                                                                               			    \
    }                                                                                   			    \
    Deque_##t##_swap(array, i+1, f);                                                    			    \
    return i+1;                                                                         			    \
}				                                                                                        \
void Deque_##t##_quicksort(t* array, bool (*less)(const t&, const t&), int b, int f) {			        \
    if(b < f) {																						    \
        int pi = Deque_##t##_partition(array, less, b, f);										        \
        Deque_##t##_quicksort(array, less, b, pi-1);												    \
        Deque_##t##_quicksort(array, less, pi+1, f);												    \
    }																								    \
}																									    \
																										\
void Deque_##t##_sort(Deque_##t* deq, Deque_##t##_Iterator front, Deque_##t##_Iterator end) {		    \
    if(deq->curSize <= 1) return;																	    \
                                                                                                        \
    int bi = (end.index + 1) % deq -> capacity;														    \
    int fi = front.index;																			    \
                                                                                                        \
    if(deq-> insertFront <= deq-> insertBack) {                                                         \
        assert(deq-> insertFront >= 0);                                                                 \
        if(deq -> curSize != deq-> capacity && deq -> insertBack != deq -> capacity - 1) {              \
            t* temp_array = (t*) malloc(deq-> curSize * sizeof(t));              					    \
            int num_back = deq-> capacity - deq-> insertBack - 1;                                       \
                                                                                                        \
            int b = (deq-> insertBack + 1) % deq-> capacity;											\
            for(int i=0; i<num_back; i++) {                                                             \
                temp_array[i] = deq->data[b];                                                     	    \
                b = (b + 1) % deq-> capacity;                                                     	    \
            }                                                                                           \
            int f = 0;																				    \
            for(int i=0; i<deq->insertFront; i++) {                                                     \
                temp_array[num_back + i] = deq->data[f];                                          	    \
                f = (f + 1) % deq-> capacity;;                                                          \
            }                                                                                           \
            for(size_t i=0; i< deq->curSize; i++) {                                                     \
                deq->data[i] = temp_array[i];                                                           \
            }                                                                                           \
            free(temp_array);                                                                           \
                                                                                                        \
            fi = (front.index + num_back) % deq-> capacity;											    \
            bi = (end.index + 1 + num_back) % deq-> capacity;										    \
            deq->insertFront = deq-> curSize;                                                           \
            deq-> insertBack = deq-> capacity - 1;                                                      \
        }                                                                                               \
    }	                                                                                                \
                                                                                                        \
    Deque_##t##_quicksort(deq->data, deq->less, bi, fi);                                      	        \
}																									    \
size_t Deque_##t##_size(Deque_##t * deq)                                                       			\
{                                                                                           			\
    return deq->curSize;                                                                    			\
}                                                                                           			\
                                                                                            			\
bool Deque_##t##_empty(Deque_##t * deq)                                                     			\
{                                                                                           			\
    return (deq->curSize == 0);                                                              			\
}                                                                                           			\
                                                                                         			    \
void Deque_##t##_expandSize(Deque_##t * deq)                                       			            \
{                                                                                         			    \
   assert(deq->curSize == deq->capacity);														        \
   size_t old_size = deq->capacity;															            \
   deq->capacity *= 2;																			        \
   deq->data = (t*) realloc(deq->data, deq->capacity * sizeof(t));								        \
   int new_back_ptr = deq->capacity - 1;															    \
   int lower_bound = deq->insertBack;																    \
   assert(lower_bound >=0);																		        \
   if((size_t)lower_bound == old_size-1) lower_bound = -1;										        \
   for(int i = old_size-1; i > lower_bound; i--) {													    \
       deq->data[new_back_ptr] = deq->data[i];													    \
       new_back_ptr--;																				    \
   }																								    \
   deq->insertBack = new_back_ptr;	                                                                   	\
}                                                                                           			\
                                                                                            			\
void Deque_##t##_push_back(Deque_##t * deq, const t v)                                          		\
{                                                                                           			\
    if(deq->curSize == deq->capacity) {															        \
		Deque_##t##_expandSize(deq);															        \
	}																								    \
																										\
    deq->data[deq->insertBack] = v;																	    \
    deq->insertBack--;																				    \
    if(deq->insertBack < 0) deq->insertBack = deq->capacity - 1;									   	\
    deq->curSize++;	                                                                           		    \
}                                                                                           			\
                                                                                            			\
void Deque_##t##_push_front(Deque_##t * deq, const t v)                                         		\
{                                                                                                       \
    if(deq->curSize == deq->capacity) {															        \
		Deque_##t##_expandSize(deq);															        \
    }																								    \
																										\
    deq->data[deq->insertFront] = v;																	\
    deq->insertFront = (deq->insertFront + 1) % deq->capacity;											\
    deq->curSize++;	                                                                                    \
}                                                                                                       \
                                                                                                        \
void Deque_##t##_pop_front(Deque_##t * deq)                                         	                \
{                                                                                                       \
    if(deq->capacity == 0) return;																	    \
																										\
    deq->insertFront--;																				    \
    if(deq->insertFront < 0) deq->insertFront = deq->capacity - 1;										\
    deq->curSize--;																				        \
                                                                                                        \
    if(deq->empty(deq)) {																			    \
        deq->insertFront = 0;																			\
        deq->insertBack = deq->capacity - 1;															\
    }																	                                \
}                                                                                                       \
void Deque_##t##_pop_back(Deque_##t * deq)                                                         		\
{                                                                                                       \
	if(deq->curSize == 0) return;																	    \
                                                                                                        \
    deq->insertBack = (deq->insertBack + 1) % deq->capacity;											\
    deq->curSize--;																				        \
                                                                                                        \
    if(deq->empty(deq)) {																			    \
        deq->insertFront = 0;																			\
        deq->insertBack = deq->capacity - 1;															\
    }														                                            \
}                                                                                                       \
												                                                        \
t& Deque_##t##_front(Deque_##t * deq)                                                                   \
{                                                                                                       \
    int ptr = deq->insertFront - 1;																	    \
    if(ptr < 0) ptr = deq->capacity - 1;															    \
	return deq->data[ptr];	                                                                            \
}                                                                                                       \
                                                                                                        \
t& Deque_##t##_back(Deque_##t * deq)                                                                    \
{                                                                                                       \
    int ptr = deq->insertBack + 1;																	    \
    if((size_t)ptr == deq->capacity) ptr = 0;														    \
    return deq->data[ptr];			                                                                    \
}                                                                                                       \
                                                                                                        \
t& Deque_##t##_at(Deque_##t * deq, size_t st)                                                           \
{   												                                                    \
	int idx = deq->insertFront; 																		\
		idx--;																							\
		if(idx < 0) idx = deq->capacity - 1;															\
																										\
		if(idx - st < 0) idx = (deq->capacity - (st - idx));							                \
		else idx = idx - st;																	        \
																										\
		return deq->data[idx];	                                                                        \
}                                                                                                       \
                                                                                                        \
void Deque_##t##_clear(Deque_##t * deq)                                                                 \
{                                                                                                       \
    if(deq->capacity == 0) return;																	    \
    deq->capacity = 0;																				    \
    deq->insertFront = 0;																				\
    deq->insertBack = deq->capacity - 1;                                                                \
}                                                                                                       \
                                                                                                        \
void Deque_##t##_dtor(Deque_##t * deq)                                                                  \
{                                                                                                       \
    free(deq->data);                                                                                    \
}                                                                                                       \
                                                                                                        \
bool Deque_##t##_equal(Deque_##t deq1, Deque_##t deq2)     				                                \
{                                                                                                       \
	if(deq1.curSize != deq2.curSize) return false;												        \
    if(deq1.less != deq2.less) return false;													        \
    if(deq1.curSize == 0) return true;															        \
                                                                                                        \
    int p1 = deq1.insertFront, p2=deq2.insertFront;														\
    for(int i = 0; i < (int)deq1.curSize; i++) {														\
        p1--;																						    \
        if(p1 < 0) p1 = deq1.capacity - 1;															    \
        p2--;																						    \
        if(p2 < 0) p2 = deq2.capacity - 1;															    \
                                                                                                        \
        t x1 = deq1.data[p1];																		    \
        t x2 = deq2.data[p2];                                                    					    \
        if(deq1.less(x1, x2)) return false;                                          			        \
        if(deq1.less(x2, x1)) return false;														        \
    }                                                                                   			    \
                                                                                                        \
    return true;        									                                            \
}                                                                                                 		\
													                                                    \
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2)			            \
{												                                                        \
    return (it1.index == it2.index) && (it1.toEnd == it2.toEnd);					                    \
}                                                                                   		            \
                                                                                                        \
void Deque_##t##_Iterator_inc(Deque_##t##_Iterator * it)                                                \
{                                                                                                       \
	if(it->toEnd == 0) return;																	        \
    it->index--;																					    \
    if(it->index < 0) it->index = it->deq->capacity - 1;											    \
    it->toEnd--;									                                                    \
}                                                                                                       \
                                                                                                        \
t& Deque_##t##_Iterator_deref(Deque_##t##_Iterator * it)                                                \
{                                                                                                       \
	return it->deq->data[it->index];			                                                        \
}                                                                                                       \
                                                                                                        \
void Deque_##t##_Iterator_dec(Deque_##t##_Iterator * it)                                                \
{                                                                                                       \
	it->index = (it->index + 1) % it->deq->capacity;												    \
	it->toEnd++;									                                                    \
}                                                                                                       \
											                                                            \
Deque_##t##_Iterator Deque_##t##_begin(Deque_##t * deq)                                                 \
{                                                                                                       \
    if(deq->curSize == 0) return deq->end(deq);													        \
    Deque_##t##_Iterator it;																		    \
    it.deq = deq;																					    \
    if(deq->insertFront == 0) it.index = deq->capacity - 1;											    \
    else it.index = deq->insertFront - 1;																\
    it.toEnd = deq->curSize; 																            \
                                                                                                        \
    it.inc = Deque_##t##_Iterator_inc;																    \
    it.dec = Deque_##t##_Iterator_dec;																    \
    it.deref = Deque_##t##_Iterator_deref;															    \
    return it;	                                                                                        \
}                                                                                                       \
                                                                                                        \
Deque_##t##_Iterator Deque_##t##_end(Deque_##t * deq)                                                   \
{                                                                                                       \
    Deque_##t##_Iterator it;																		    \
    it.deq = deq;																					    \
    it.index = deq->insertBack;																		    \
    it.toEnd = 0;																				        \
                                                                                                        \
    it.inc = Deque_##t##_Iterator_inc;																    \
    it.dec = Deque_##t##_Iterator_dec;																    \
    it.deref = Deque_##t##_Iterator_deref;															    \
    return it;		                                                                                    \
}                                                                                                       \
                                                                                                        \
void Deque_##t##_ctor(Deque_##t * deq, bool ( *comaperFunc ) (const t &, const t &))                    \
{                                                                                                       \
    strcat(deq->type_name, #t);                                                                         \
    deq->curSize = 0;                                                                                   \
    deq->capacity = 20;                                                                                 \
    deq->data = (t*) malloc (deq->capacity * sizeof(t));                                                \
    deq->insertFront = 0;                                                                               \
    deq->insertBack = 19;                                                                               \
                                                                                                        \
    deq->size = &Deque_##t##_size;                                                                      \
    deq->empty = &Deque_##t##_empty;                                                                    \
    deq->push_back = &Deque_##t##_push_back;                                                            \
    deq->push_front = &Deque_##t##_push_front;                                                          \
    deq->front = &Deque_##t##_front;                                                                    \
    deq->back = &Deque_##t##_back;                                                                      \
    deq->pop_front = &Deque_##t##_pop_front;                                                            \
    deq->pop_back = &Deque_##t##_pop_back;                                                              \
    deq->begin = &Deque_##t##_begin;                                                                    \
    deq->end = &Deque_##t##_end;                                                                        \
    deq->at = &Deque_##t##_at;                                                                          \
    deq->clear = &Deque_##t##_clear;                                                                    \
    deq->dtor = &Deque_##t##_dtor;                                                                      \
    deq->less = *comaperFunc;                                                                           \
    deq->sort = &Deque_##t##_sort;				            		                                    \
}                                                                                                       \

#endif // DEQUE_H 