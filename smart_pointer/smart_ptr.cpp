#include <iostream>
#include <stdexcept>
using namespace std;

class null_ptr_exception : public std::runtime_error
{
public:
	//constructor for custom exception
	//s in this case acts like an overload for the what and throw function
	null_ptr_exception(const char* s) : runtime_error(s) {}
};


//make sure not to accesss or change garbage
template <typename T>
class smart_ptr {
private:
	T* ptr_;               // pointer to the referred object
	int* ref_;             // pointer to a reference count
	//dont include custom exception here because other people need to access it

public:
	smart_ptr() noexcept : ptr_(nullptr), ref_(nullptr)  {}

	// Create a smart_ptr that is initialized to nullptr. The reference count
	// should be initialized to nullptr.

	explicit smart_ptr(T*& raw_ptr) : ptr_(nullptr), ref_(nullptr) {
		ptr_ = raw_ptr;
		ref_ = new int(1);
	}

	// Create a smart_ptr that is initialized to raw_ptr. The reference count
	// should be one. No change is made to raw_ptr.

	explicit smart_ptr(T*&& raw_ptr) noexcept : ptr_(nullptr), ref_(nullptr)  {
		ptr_ = raw_ptr;
		try {
			ref_ = new int(1);
		}
		catch (const std::bad_alloc&) {
			delete raw_ptr;
			throw;
		}
	}

	// Create a smart_ptr that is initialized to raw_ptr. The reference count
	// should be one. If the constructor fails raw_ptr is deleted.

	smart_ptr(const smart_ptr& rhs) noexcept {
		if (rhs.ptr_ == nullptr) {
			ptr_ = nullptr;
			ref_ = nullptr;
		}
		else
		{
			ptr_ = rhs.ptr_;
			ref_ = rhs.ref_;
			(*ref_)++;
		}
	}

	// Copy construct a pointer from rhs. The reference count should be
	// incremented by one.

	smart_ptr(smart_ptr&& rhs) noexcept {
		ptr_ = rhs.ptr_;
		ref_ = rhs.ref_;
		rhs.ptr_ = nullptr;
		rhs.ref_ = nullptr;
	}

	// Move construct a pointer from rhs.

	smart_ptr& operator=(const smart_ptr& rhs) noexcept {
		if (this == &rhs)
			return *this;
		if (ref_ != nullptr) {
			if (ref_count() <= 1) {
				delete ptr_;
				delete ref_;
			}
			else
				(*ref_)--;
		}

		ptr_ = rhs.ptr_;
		ref_ = rhs.ref_;
		if (ref_ != nullptr)
			(*ref_)++;

		return *this;
	}

	// This assignment should make a shallow copy of the right-hand side's
	// pointer data. The reference count should be incremented as appropriate.

	smart_ptr& operator=(smart_ptr&& rhs) noexcept {
		if (this == &rhs)
			return *this;
		if (ref_ != nullptr) {
			if (ref_count() <= 1) {
				delete ptr_;
				delete ref_;
			}
			else
				(*ref_)--;
		}

		ptr_ = rhs.ptr_;
		ref_ = rhs.ref_;
		rhs.ptr_ = nullptr;
		rhs.ref_ = nullptr;
		return *this;

	}

	// This move assignment should steal the right-hand side's pointer data.

	bool clone() {
		if (ptr_ == nullptr || (*ref_) == 1)
			return false;
		else {
			try
			{
				// This if statement is kind of obsolete, but its an extra layer of protection
				if (ref_ != nullptr) {
					T* tempPtr_ = new T(*ptr_);
					int* tempRef_ = new int(1);
					(*ref_)--;
					ptr_ = tempPtr_;
					ref_ = tempRef_;
				}
				else
					throw null_ptr_exception("Can't dereference nullptr");
			}
			catch (const std::bad_alloc&)
			{
				throw;
			}
		}
		return true;
	}

	// If the smart_ptr is either nullptr or has a reference count of one, this
	// function will do nothing and return false. Otherwise, the referred to
	// object's reference count will be decreased and a new deep copy of the
	// object will be created. This new copy will be the object that this
	// smart_ptr points and its reference count will be one.

	int ref_count() const noexcept {
		return ((ref_ != nullptr) ? (*ref_) : 0);
	}

	// Returns the reference count of the pointed to data.

	T& operator*() {
		if (ptr_ == nullptr)
			throw null_ptr_exception("Can't dereference nullptr");
		return *ptr_;
	}

	// The dereference operator shall return a reference to the referred object.
	// Throws null_ptr_exception on invalid access.

	T* operator->() {
		if (ptr_ == nullptr)
			throw null_ptr_exception("Can't dereference nullptr");
		return ptr_;
	}

	// The arrow operator shall return the pointer ptr_. Throws null_ptr_exception
	// on invalid access.

	~smart_ptr() noexcept {

		if (ref_ != nullptr) {
			if ((*ref_) <= 1) {
				delete ptr_;
				delete ref_;
			}
			else
				(*ref_)--;
		}
	}
	//only deallocate if the control block has a int <= 1
	// deallocate all dynamic memory


};



int main() {

	int* p{ new int { 42 } };
	smart_ptr<int> sp1{ p };

	cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 1
	{
		smart_ptr<int> sp2{ sp1 };
		cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 2
		cout << "Ref count is " << sp2.ref_count() << endl;	// Ref Count is 2
	}
	cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 1

	smart_ptr<int> sp3;

	cout << "Ref count is " << sp3.ref_count() << endl;	// Ref Count is 0

	sp3 = sp1;

	cout << "Ref count is " << sp1.ref_count() << endl;	// Ref Count is 2
	cout << "Ref count is " << sp3.ref_count() << endl;	// Ref Count is 2

	smart_ptr<int> sp4 = std::move(sp1);

	cout << *sp4 << " " << *sp3 << endl;        // prints 42 42
	cout << "Passed" << endl;

	try
	{
		cout << *sp1 << endl;
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}


	smart_ptr<double> dsp1{ new double {3.14} };
	smart_ptr<double> dsp2, dsp3;

	dsp3 = dsp2 = dsp1;

	cout << dsp1.ref_count() << " " << dsp2.ref_count() << " " << dsp3.ref_count() << endl;
	// prints 3 3 3
	cout << *dsp1 << " " << *dsp2 << " " << *dsp3 << endl;
	// prints 3.14 3.14 3.14

	cout << dsp1.clone() << endl;        // returns true

	cout << dsp1.ref_count() << " " << dsp2.ref_count() << " " << dsp3.ref_count() << endl;
	// prints 1 2 2
	cout << *dsp1 << " " << *dsp2 << " " << *dsp3 << endl;
	// prints 3.14 3.14 3.14

	cout << "Passed" << endl;
}
