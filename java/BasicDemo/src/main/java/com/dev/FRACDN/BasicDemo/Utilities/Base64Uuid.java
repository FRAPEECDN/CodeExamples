package com.dev.FRACDN.BasicDemo.Utilities;

import org.springframework.stereotype.Component;

import java.nio.ByteBuffer;
import java.util.Base64;
import java.util.UUID;

/**
 * Utility component to assist with UUID generation
 */
@Component
public class Base64Uuid {

    private static final Base64Uuid instance = new Base64Uuid();

    private final Base64.Encoder encoder;

    public Base64Uuid() {
        this.encoder = Base64.getEncoder();
    }

    /**
     * @return UUID generated as string
     */
    public String newUuid() {
        UUID uuid = UUID.randomUUID();

        byte[] src = ByteBuffer.wrap(new byte[16])
                .putLong(uuid.getMostSignificantBits())
                .putLong(uuid.getLeastSignificantBits())
                .array();

        return encoder.encodeToString(src);
    }

    public static Base64Uuid getInstance() {
        return instance;
    }
}
