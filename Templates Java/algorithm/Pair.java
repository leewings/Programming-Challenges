package algorithm;

public class Pair<T, U> {

    public T a;
    public U b;

    Pair(T a, U b) {
        this.a = a;
        this.b = b;
    }

    public static <T, U> Pair<T, U> of(T a, U b) {
        return new Pair<>(a, b);
    }

    public T getA() {
        return a;
    }

    public U getB() {
        return b;
    }

    @Override
    public String toString() {
        return String.format("(%s, %s)", a, b);
    }

}