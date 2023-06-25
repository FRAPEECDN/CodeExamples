package com.dev.FRACDN.BasicDemo.Model.Parameters;

import com.dev.FRACDN.BasicDemo.Model.BasicEnum;
import jakarta.validation.constraints.*;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * Parameter object for Basic
 */
@EqualsAndHashCode(callSuper = false)
@Data
public class BasicParam extends BaseParam {

    @NotBlank
    @Size.List({
            @Size(min = 2, message = "{validation.name.size.too_short}"),
            @Size(max = 64, message = "{validation.name.size.too_long}")
    })
    private String description;

    @Size(min = 0, max = 100)
    private Integer number;

    @NotNull
    private Boolean option;

    @NotNull
    private BasicEnum basicEnum;
}
