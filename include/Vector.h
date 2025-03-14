#pragma once

template<typename T>
class Vector {
  public:
    Vector() : size_(0), capacity_(0), data_(0) {}
    Vector(const Vector& b){
      
      size_ = b.size_;
      capacity_ = b.capacity_;

      data_ = malloc(sizeof(T)*capacity_);
      memcpy(data_, b.data_, sizeof(T)*capacity_);

    }
    ~Vector() 
    {
      delete[] data_;
    }

    void resize() 
    {
      long int new_capacity = (capacity_ == 0 ? 400 : 2 * capacity_);
      T *new_data = new T[new_capacity];
      for (int i = 0; i < size_; i++) 
      {
        new_data[i] = data_[i];
      }
      capacity_ = new_capacity;
      delete[] data_;
      data_ = new_data;
    }

    void push_back(const T &value) 
    {
      if (size_ >= capacity_) {
        resize();
      }
      data_[size_++] = value;
    }

    void reverse() 
    {
      T nn;
      for (int i = 0; i < size_ / 2; i++) {
        nn = data_[i];
        data_[i] = data_[size_ - i - 1];
        data_[size_ - i - 1] = nn;
      }
    }

    void pop_back() 
    {
      size_--;
    }

    void remove(int index)
    {
      for(size_t i = index; i < size_; i++)
      {
        data_[i] = data_[i + 1];
      }
      size_--;
    }

    void clear() 
    {
      size_ = 0;
    }

    int size() const 
    {
      return size_;
    }
    int capacity() const 
    {
      return capacity_;
    }

    Vector& operator = (const Vector& b){
      
      size_ = b.size_;
      capacity_ = b.capacity_;

      delete[] data_;

      data_ = malloc(sizeof(T)*capacity_);
      memcpy(data_, b.data_, sizeof(T)*capacity_);

      return *this;
    }

    T &operator[](int index)
    {
      return data_[index];
    }
    const T &operator[](int index) const 
    {
      return data_[index];
    }
    T* begin()
    {
      return &data_[0];
    }
  private:
    long int size_;
    long int capacity_;
    T *data_;
};