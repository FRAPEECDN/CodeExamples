package com.dev.FRACDN.BasicDemo.Model.DTO;

import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * Complete Object - DTO for the string example
 */
@EqualsAndHashCode(callSuper = false)
@Data
public class CompleteDTO extends BaseObject {

    private String publicId;

    private String description;

}
