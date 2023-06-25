import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

/**
 * Basic POJO class as example, using Lombok for boilerplate code
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
public class PojoExample {

    private int number;
    private double decimalNumber;
    private LocalDateTime localDateTime;
    private String someString;
    private boolean option;
    private PlainEnum anEnum;

}
