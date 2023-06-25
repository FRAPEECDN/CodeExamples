package com.dev.FRACDN.BasicDemo.Integration.Mvc;

import com.dev.FRACDN.BasicDemo.Controller.TestController;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;

import static org.hamcrest.Matchers.equalTo;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

/**
 * Most simple test for the API. to see if test does respond correctly
 */
@SpringBootTest
@AutoConfigureMockMvc
public class TestAllControllersActive {

    @Autowired
    private MockMvc mvc;

    @Test
    public void testController() throws Exception {
        mvc.perform(MockMvcRequestBuilders
                        .get("/test")
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(content().string(equalTo(TestController.MESSAGE)));
    }

    @Test
    public void crudController() throws Exception {
        mvc.perform(MockMvcRequestBuilders
                        .get("/crud")
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(content().string(equalTo("[]")));
    }

    @Test
    public void simpleController() throws Exception {
        mvc.perform(MockMvcRequestBuilders
                        .get("/simple")
                        .accept(MediaType.APPLICATION_JSON))
                .andExpect(status().isOk())
                .andExpect(content().string(equalTo("[]")));
    }


}
