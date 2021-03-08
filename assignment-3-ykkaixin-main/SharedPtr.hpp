
#include <iostream>
#include <assert.h>
#include <pthread.h>
#include <mutex>

namespace cs540
{
    std::mutex countLock;

    class Helper {
        public:
            int ref_count;
            Helper() {}
            virtual void deleter () {};
            virtual ~ Helper() {}
    };

    template<typename T>
    class HelperDerived final: public Helper {
        public:
            T *ptr;

            HelperDerived(T *p) {ptr = p;}
            void deleter() {delete ptr;}
            ~ HelperDerived() {}
    };

    template<typename T>
    class SharedPtr {
        public:
            T *ptr;
            Helper *help_obj; // count object

            SharedPtr() {
                this -> help_obj = nullptr;
                this -> ptr = nullptr;
            }

            template<typename X> explicit
            SharedPtr(X *p) {
                if(p) {
                    this -> help_obj = new HelperDerived<X>(p);
                    this -> help_obj -> ref_count = 1;
                    this -> ptr = p;
                }
            }

            SharedPtr(const SharedPtr &p) {
                if(p.ptr) { 
                    this -> ptr = p.ptr;
                    this -> help_obj = p.help_obj;

                    countLock.lock();
                    p.help_obj -> ref_count++;
                    countLock.unlock();
                } else {
                    this -> help_obj = nullptr;
                    this -> ptr = nullptr;
                }
            }

            template <typename X> 
			SharedPtr (const SharedPtr <X> & p) {
				if (p.ptr) {   
                    this -> ptr = p.ptr;
					this -> help_obj = p.help_obj;

					countLock.lock();
					p.help_obj -> ref_count++;
					countLock.unlock();
				} else {
					this -> ptr = nullptr;
					this -> help_obj = nullptr;
				}
			}

            SharedPtr <T> &
			operator = (const SharedPtr & p){
				//printf("Copy assignment operator\n");
				countLock.lock();

				if (this -> ptr) {
					this -> help_obj -> ref_count--;

					if (this -> help_obj -> ref_count == 0) {
						this -> help_obj -> deleter();
						this -> ptr = nullptr;

						delete this -> help_obj;
					}
				}

				if (p.ptr) {
					p.help_obj -> ref_count++;
					this -> ptr = p.ptr;
					this -> help_obj = p.help_obj;
				} else {
					this -> help_obj = nullptr;
					this -> ptr = nullptr;
				}

				countLock.unlock();

				return (* this);
			}

            template <typename X>
            SharedPtr <T> &
            operator = (const SharedPtr <X> &p) {
                countLock.lock();
                if (this -> ptr) 
				{
					this -> help_obj -> ref_count--;

					if (this -> help_obj -> ref_count == 0) 
					{
						this -> help_obj -> deleter();
						this -> ptr = nullptr;

						delete this -> help_obj;
					}
				}

				if (p.ptr) 
				{
					p.help_obj -> ref_count++;

					this -> ptr = p.ptr;
					this -> help_obj = p.help_obj;
				}
				else
				{
					this -> help_obj = nullptr;
					this -> ptr = nullptr;
				}
                countLock.unlock();
                return (* this);
            }

            SharedPtr <T> &
			operator = (SharedPtr && p)				// Move assignment operator
			{
				//printf("Move assignment operator\n");
				if (p.ptr)
				{
					this -> ptr = p.ptr;
					this -> help_obj = p.help_obj;

					p.ptr = nullptr;
				}
				else
				{
					this -> help_obj = nullptr;
					this -> ptr = nullptr;
				}

				return (* this);
			}

			template <typename X>
			SharedPtr <T> &
			operator = (SharedPtr <X> && p)			// Move assignment operator with different type
			{
				if (p.ptr)
				{
					this -> ptr = p.ptr;
					this -> help_obj = p.help_obj;

					p.ptr = nullptr;
				}
				else
				{
					this -> help_obj = nullptr;
					this -> ptr = nullptr;
				}
				
				return (* this);
			}

            ~ SharedPtr ()							// Destructor	
			{
				countLock.lock();

				if (this -> help_obj) {
					this -> help_obj -> ref_count--;

					if (this -> help_obj -> ref_count == 0) 
					{
						this -> help_obj -> deleter();
						delete this -> help_obj;
					}
					else if (this -> help_obj -> ref_count < 0) assert(false);
				}
				
				countLock.unlock();
			}

            void reset (){
				countLock.lock();

				if (this -> help_obj) 
				{
					this -> help_obj -> ref_count--;

					if (this -> help_obj -> ref_count == 0) 
					{
						this -> help_obj -> deleter();
						delete this -> help_obj;
					}
					else if (this -> help_obj -> ref_count < 0) assert(false);
				}

				this -> help_obj = nullptr;
				this -> ptr = nullptr;
				
				countLock.unlock();
			}

			template <typename X> void reset (X * p) {
				countLock.lock();

				if (this -> help_obj){
					this -> help_obj -> ref_count--;

					if (this -> help_obj -> ref_count == 0){
						this -> help_obj -> deleter();
						delete this -> help_obj;
					}
					else if (this -> help_obj -> ref_count < 0) assert(false);					
				}

				if (p){
					this -> help_obj = new HelperDerived <X> (p);
					this -> help_obj -> ref_count = 1;

					this -> ptr = p;
				}
				
				countLock.unlock();
			}

			T * get () const {return this -> ptr;}

			T &operator * () const {return * (this -> ptr);}

			T *operator -> () const {return this -> ptr;}

			explicit operator bool () const {	
				if (this -> ptr) return true;
				else return false;
			}
    };

    template <typename T1, typename T2>
	bool operator == (const SharedPtr <T1> & sp1, const SharedPtr <T2> & sp2) {
		if (sp1.ptr == sp2.ptr) return true;
		if (sp1.ptr == nullptr && sp2.ptr == nullptr) return true;

		return false;
	}

	template <typename T>
	bool operator == (const SharedPtr <T> & sp, std::nullptr_t) {
		if (sp.ptr == nullptr) return true;
		else return false;
	}

	template <typename T>
	bool operator == (std::nullptr_t, const SharedPtr <T> & sp)
	{
		if (sp.ptr == nullptr) return true;
		else return false;
	}

	template <typename T1, typename T2>
	bool operator != (const SharedPtr <T1> & sp1, const SharedPtr <T2> & sp2) {
		if (sp1.ptr != sp2.ptr) return true;
		if (sp1.ptr && sp2.ptr == nullptr) return true;
		if (sp2.ptr && sp1.ptr == nullptr) return true;

		return false;
	}

	template <typename T>
	bool operator != (const SharedPtr <T> & sp, std::nullptr_t) {
		if (sp.ptr == nullptr) return false;
		else return true;
	}

	template <typename T>
	bool operator != (std::nullptr_t, const SharedPtr <T> & sp)	{
		if (sp.ptr == nullptr) return false;
		else return true;
	}

	template <typename T, typename U>
	SharedPtr <T> static_pointer_cast (const SharedPtr <U> & sp) {
		SharedPtr <T> s;
		s.ptr = static_cast <T *> (sp.ptr);
		return s;
	}

	template <typename T, typename U>
	SharedPtr <T> dynamic_pointer_cast (const SharedPtr <U> & sp) {
		SharedPtr <T> s;
		s.ptr = dynamic_cast <T *> (sp.ptr);
		return s;
	}
}