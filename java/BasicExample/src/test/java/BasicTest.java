import dev.FRACDN.ExampleStruct;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class BasicTest {

    int addExample(int a, int b) {
        return a + b;
    }

    @Test
    public void testMethod1() {
        assertEquals(addExample(1, 2), 3);
        assertNotEquals(addExample(1, 2), 4);
    }

    @Test
    public void testMethod2() {
        int a = 3;
        int b = 4;
        assertNotEquals(a, b);
        b = 3;
        assertEquals(a, b);
    }

    @Test
    public void testMethod3() {
        ExampleStruct a = new ExampleStruct(5,"abc");
        ExampleStruct b = new ExampleStruct(6, "def");
        assertNotNull(a);
        assertNotEquals(a, b);
    }
}
