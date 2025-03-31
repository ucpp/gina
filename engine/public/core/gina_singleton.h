#ifndef _GINA_SINGLETON_H_
#define _GINA_SINGLETON_H_

namespace gina
{
    template<typename T>
    class Singleton 
    {
    public:
        static T& Get() 
        {
            static T instance;
            return instance;
        }

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

    protected:
        Singleton() = default;
        ~Singleton() = default;
    };
}

#endif // !_GINA_SINGLETON_H_