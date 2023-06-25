package com.dev.FRACDN.BasicDemo.Controller;

import com.dev.FRACDN.BasicDemo.Service.SimpleService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;

import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;

/**
 * Example for writing a specific controller unit test with mocking iy service
 */
@WebMvcTest(SimpleController.class)
public class SimpleControllerTest {

    private static final String PATH = "/simple";

    @MockBean
    private SimpleService simpleService;

    @Autowired
    private MockMvc mvc;

    @Test
    public void testGetAll() throws Exception {
        List<String> setupData = List.of(
                "apple",
                "pear",
                "peach",
                "grape",
                "orange"
        );

        when(simpleService.getAll()).thenReturn(setupData);
        final MvcResult result = mvc.perform(MockMvcRequestBuilders.get(PATH)
                .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andReturn();
        assertThat(result.getResponse().getContentType()).isNotNull();

    }
}
