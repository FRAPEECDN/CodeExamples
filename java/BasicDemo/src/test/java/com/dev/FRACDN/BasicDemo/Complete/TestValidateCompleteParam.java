package com.dev.FRACDN.BasicDemo.Complete;

import com.dev.FRACDN.BasicDemo.Model.Parameters.CompleteParam;
import jakarta.validation.ConstraintViolation;
import jakarta.validation.Validation;
import jakarta.validation.Validator;
import jakarta.validation.ValidatorFactory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.Set;

import static org.assertj.core.api.Assertions.assertThat;

@ExtendWith(MockitoExtension.class)
public class TestValidateCompleteParam {

    private Validator validator;

    @BeforeEach
    public void setUp() {
        ValidatorFactory factory = Validation.buildDefaultValidatorFactory();
        validator = factory.getValidator();
    }

    @Test
    public void testValidParameter() {
        CompleteParam newParam = new CompleteParam();
        newParam.setDescription("How");
        Set<ConstraintViolation<CompleteParam>> violations = validator.validate(newParam);
        assertThat(violations).isEmpty();
    }

    @Test
    public void testParameterAsNull() {
        CompleteParam newParam = new CompleteParam();
        newParam.setDescription(null);
        Set<ConstraintViolation<CompleteParam>> violations = validator.validate(newParam);
        assertThat(violations).isNotEmpty();
        assertThat(violations.stream()
                .anyMatch(violation -> violation.getMessage().equals(CompleteParam.BLANK_DESCRIPTION))
        ).isTrue();
    }

    @Test
    public void testParameterAsBlank() {
        CompleteParam newParam = new CompleteParam();
        newParam.setDescription("");
        Set<ConstraintViolation<CompleteParam>> violations = validator.validate(newParam);
        assertThat(violations).isNotEmpty();
        assertThat(violations.stream()
                .anyMatch(violation -> violation.getMessage().equals(CompleteParam.BLANK_DESCRIPTION))
        ).isTrue();
    }

    @Test
    public void testParameterAsTooShort() {
        CompleteParam newParam = new CompleteParam();
        newParam.setDescription("1");
        Set<ConstraintViolation<CompleteParam>> violations = validator.validate(newParam);
        assertThat(violations).isNotEmpty();
        assertThat(violations.stream()
                .anyMatch(violation -> violation.getMessage().equals(CompleteParam.SHORT_DESCRIPTION))
        ).isTrue();   }

    @Test
    public void testParameterAsTooLong() {
        CompleteParam newParam = new CompleteParam();
        newParam.setDescription("012345678901");
        Set<ConstraintViolation<CompleteParam>> violations = validator.validate(newParam);
        assertThat(violations).isNotEmpty();
        assertThat(violations.stream()
                .anyMatch(violation -> violation.getMessage().equals(CompleteParam.LONG_DESCRIPTION))
        ).isTrue();
    }

}
