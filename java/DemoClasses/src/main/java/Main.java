import java.time.LocalDateTime;

public class Main {
    public static void main(String[] args) {
        PojoExample example1 = new PojoExample();
        example1.setSomeString("Some string example");
        example1.setNumber(100);
        example1.setDecimalNumber(362.5487);
        example1.setLocalDateTime(LocalDateTime.now());
        example1.setOption(true);
        example1.setAnEnum(PlainEnum.OPTION_1);

        PojoExample example2 = new PojoExample(50, 38.58, LocalDateTime.now(), "Some text for example 2", false, PlainEnum.OPTION_2);

        System.out.println("Example POJO (Plain Old Java Object");
        System.out.println("Example 1 is");
        System.out.println(example1);

        System.out.println("Example 2 is");
        System.out.println(example2);
        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        System.out.println("Comparison between the 2 examples");
        System.out.println("The same ? : " + example1.equals(example2));

    }
}
