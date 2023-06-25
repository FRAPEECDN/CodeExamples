package com.dev.FRACDN.BasicDemo.Service;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.junit.jupiter.api.Assertions.assertThrows;

/**
 * Testing the simple service directly (to make sure that layer works)
 */
@ExtendWith(MockitoExtension.class)
public class SimpleServiceTest {

    @InjectMocks
    private SimpleService simpleService;

    @BeforeEach
    void setUp() {
        List<String> setupData = List.of(
                "apple",
                "pear",
                "peach",
                "grape",
                "orange"
        );
        simpleService.setInternalStrings(setupData);
    }

    @AfterEach
    void tearDown() {
        simpleService.resetRepositoryList();
    }

    @Test
    public void testGetAll() {
        List<String> results = simpleService.getAll();
        assertThat(results.size()).isEqualTo(5);
        simpleService.resetRepositoryList();
        results = simpleService.getAll();
        assertThat(results).isEmpty();
    }

    @Test
    public void testGetOne() {
        String result = simpleService.findById(1);
        assertThat(result).isNotNull();
        assertThat(result).isNotBlank();
    }

    @Test
    public void testCreate() {
        int countStart = simpleService.getAll().size();
        String value = "Coconut";
        int indexResult = simpleService.create(value);
        assertThat(indexResult).isGreaterThan(0);
        String testAdded = simpleService.findById(indexResult);
        assertThat(testAdded).isNotNull();
        assertThat(testAdded).isNotBlank();
        int countCurrent = simpleService.getAll().size();
        assertThat(countCurrent).isGreaterThan(countStart);
    }

    @Test
    public void testUpdate() {
        String oldValue = simpleService.findById(1);
        String newValue = "Coconut";
        String updateResult = simpleService.update(1, newValue);
        assertThat(updateResult).isEqualTo(oldValue); // Please see set of List for explanation
        String currentValue = simpleService.findById(1);
        assertThat(currentValue).isNotEqualTo(oldValue);
    }

    @Test
    public void testDelete() {
        int countStart = simpleService.getAll().size();
        String oldValue = simpleService.findById(1);
        simpleService.delete(1);
        int countCurrent = simpleService.getAll().size();
        String currentValue = simpleService.findById(1);
        assertThat(countCurrent).isLessThan(countStart);
        assertThat(currentValue).isNotEqualTo(oldValue);
    }

    @Test
    public void testGetOneFail() {
        Exception exception = assertThrows(IndexOutOfBoundsException.class, () -> {
            simpleService.findById(10);
        });
        assertThat(exception.getMessage()).isNotBlank();
    }

    @Test
    public void testUpdateFail() {
        Exception exception = assertThrows(IndexOutOfBoundsException.class, () -> {
            simpleService.update(10, "Coconut");
        });
        assertThat(exception.getMessage()).isNotBlank();
    }
}
