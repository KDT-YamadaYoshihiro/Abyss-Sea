$env:ANTHROPIC_AUTH_TOKEN = "lmstudio"
$env:ANTHROPIC_BASE_URL = "http://localhost:1234"
$env:ANTHROPIC_DEFAULT_HAIKU_MODEL = "openai/gpt-oss-20b"
$env:ANTHROPIC_DEFAULT_OPUS_MODEL = "openai/gpt-oss-20b"
$env:ANTHROPIC_DEFAULT_SONNET_MODEL = "openai/gpt-oss-20b"
$env:CLAUDE_CODE_SUBAGENT_MODEL = "openai/gpt-oss-20b"
$env:CLAUDE_CODE_ATTRIBUTION_HEADER = "0"
$env:CLAUDE_CODE_ENTRYPOINT = "cli"
$env:CLAUDE_CODE_EXECPATH = "C:\Program Files\nodejs\node.exe"
$env:CLAUDECODE = "1"

C:\Users\yoshi\.bun\bin\bunx.exe @anthropic-ai/claude-code@latest --chrome