package com.dev.FRACDN.BasicDemo.Model.Parameters;

import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * Parameter object for Complete string example
 */
@EqualsAndHashCode(callSuper = false)
@Data
public class CompleteParam {

    public static final String BLANK_DESCRIPTION = "Complete param is blank or null";
    public static final String SHORT_DESCRIPTION = "Complete param is too short";
    public static final String LONG_DESCRIPTION = "Complete param is too long";

    @NotBlank(message = BLANK_DESCRIPTION)
    @Size.List({
            @Size(min = 2, message = SHORT_DESCRIPTION),
            @Size(max = 10, message = LONG_DESCRIPTION)
    })
    private String description;
}
