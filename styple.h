#if !defined(STYLE_H)
#define STYLE_H

class RootType {
public:
	RootType() {};
	virtual ~RootType() = 0;
	virtual int MethodOne() = 0;
	virtual int Size() { return sizeof(this); }
protected:

private:
	int variableOne;
	int variableTwo;
}

class NewType : public RootType {
public:
	NewType() {}
	NewType(int var) : variableOne(var) {};
	~virtual NewType() {}
	virtual in MethodOne();
	virtual int Size() { return sizeof(this); }

protected:

private:
	int variableOne;
	int variableTwo;
}

#endif // STYPLE_H
