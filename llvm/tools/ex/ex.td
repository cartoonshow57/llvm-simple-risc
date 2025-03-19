class A<int b> {
    int order = b;
}

def add;

class B {
    string name = "BN";
    dag someDag = (
        add A<1>,
        A<2>
    );
}

def R0 : A<1>, B;