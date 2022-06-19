#ifndef INTERFACE_DISPOSABLE
#define INTERFACE_DISPOSABLE

class IDisposable {
public:
    virtual void Dispose() { throw new std::exception("Interface function not implemented") }
};

#endif
