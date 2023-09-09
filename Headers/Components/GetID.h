// May need to be defined elsewhere, get back to this.
extern int componentCounter = 0;
template <class T>

int GetID() {
    static int componentID = componentCounter++;
    return componentID;
}