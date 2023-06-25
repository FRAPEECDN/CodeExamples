package com.dev.FRACDN.BasicDemo.Model.DTO;

import com.dev.FRACDN.BasicDemo.Model.BasicEnum;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * DTO of Basic Object as used in Basic Service, Controller
 */
@EqualsAndHashCode(callSuper = false)
@Data
public class BasicDTO extends BaseObject{

    private String publicId;

    private String description;

    private Integer number;

    private Boolean option;

    private BasicEnum basicEnum;

}
